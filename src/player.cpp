#include "player.h"
#include <QDataStream>
#include "compress/packer.h"
#include <stdexcept>
#include <QDebug>

#if defined(Q_OS_LINUX)

#if _POSIX_C_SOURCE  >= 199309L
#include <time.h>   // for nanosleep()
#elif
#error Need to be built with _POSIX_C_SOURCE  >= 199309L
#endif

#endif  // Q_OS_LINUX

#ifdef __linux

void __sleep_msecs(uint64_t ms) {
    struct timespec tm =
     {
       static_cast<time_t>(ms / 1000),
       static_cast<long>((ms % 1000) * 1000000L)
     };
    nanosleep(&tm, NULL);
}

#endif

using namespace MM2Capture;

Player::Player(QObject *prnt): QThread(prnt)
{ }

void
Player::startWork() {
    if (!m_pDbReader)
        throw std::runtime_error("No DBReader provided");
    m_pDbReader->open();
    m_pOutput->start();
}

void
Player::stopWork() {
    m_pDbReader->close();
    m_pOutput->stop();
}

void
Player::run() {
    try {
        startWork();
    } catch (const std::runtime_error &exc) {
        emit error( QString(exc.what()) );
        stopWork();
        return;
    }

    QByteArray currentChunk;
    QVector<ModesData> msgs;
    // if chunk's len isn't odd, stores last message from the chunk
    // to calculate timediff with first one of the next chunk
    ModesData pendingSend;
    bool pending = false;
    bool needQuit = false;

    while (!needQuit) {
        if (isInterruptionRequested()) {
            break;
        }

        if (!m_pDbReader->getNextChunk(currentChunk))
            break;

        QByteArray inflatedChunkData;
        try {
            inflatedChunkData = Packer::decompress(currentChunk);
        } catch (const std::runtime_error &exc) {
            // TODO error logging
            continue;
        }

        QDataStream strm(inflatedChunkData);
        strm.setVersion(QDataStream::Qt_4_0);
        strm >> msgs;

        QVector<ModesData>::iterator iMsg = msgs.begin();
        QVector<ModesData>::iterator msgsEnd = msgs.end();

        while (iMsg != msgsEnd) {

            if (isInterruptionRequested()) {
                needQuit = true;
                break;
            }

            if (!pending) {
                if (iMsg != msgsEnd - 1) {
                    quint64 sleepTimeMsec = (iMsg + 1)->timestamp() -
                            iMsg->timestamp();
                    *m_pOutput << *iMsg;
                    __sleep_msecs(sleepTimeMsec);
                    ++iMsg;
                } else {
                    pendingSend = *iMsg;
                    pending = true;
                    ++iMsg;
                }
            } else {
                quint64 sleepTimeMsec = iMsg->timestamp() - pendingSend.timestamp();
                pending = false;
                *m_pOutput << pendingSend;
                if (sleepTimeMsec)
                    __sleep_msecs(sleepTimeMsec);
            }
        }

        qDebug() << "chunk send";
    }
    stopWork();
}
