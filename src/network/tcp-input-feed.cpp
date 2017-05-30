#include "tcp-input-feed.h"
#include "../modes/modes-data.h"

#include <QVector>

using namespace MM2Capture;

TcpClientInputFeed::TcpClientInputFeed():
    BaseInputFeed(BaseInputSession::Ptr(new TcpInputSession(this))),
    m_pSocket{new QTcpSocket(nullptr)},
    m_pMapper{new SocketSignalMapper(this)} {
}

TcpClientInputFeed::TcpClientInputFeed(const TcpClientInputFeed &f):
    m_pSocket{new QTcpSocket(nullptr)},
    m_pMapper{new SocketSignalMapper(this)} {
    m_strHost = f.m_strHost;
    m_nPort = f.m_nPort;
}

bool
TcpClientInputFeed::operator ==(const TcpClientInputFeed &rhs) {
    return m_strHost == rhs.m_strHost &&
            m_nPort == rhs.m_nPort;
}

TcpClientInputFeed&
TcpClientInputFeed::operator =(const TcpClientInputFeed &rhs) {
    if (!(*this == rhs)) {
        TcpClientInputFeed tmp{rhs};
        qSwap(*this, tmp);
    }
    return *this;
}

void
TcpClientInputFeed::implStart() {
    m_pSocket->connectToHost(m_strHost, m_nPort);
    QObject::connect(m_pSocket.data(), SIGNAL(connected()),
                     m_pMapper.data(), SLOT(slotConnected()));
    m_pSession->setDBWriter(m_pDb);
}

void
TcpClientInputFeed::implStop() {
    m_pSocket->disconnectFromHost();
}

void TcpClientInputFeed::generateIdent()
{
    QString ident = "inConnect(%1:%2)";
    setIdent(ident.arg(m_strHost).arg(m_nPort));
}

void TcpClientInputFeed::handleConnect()
{
    QObject::connect(m_pSocket.data(), SIGNAL(readyRead()),
                     m_pSession.data(), SLOT(slotHandleRead()));
}

void
SocketSignalMapper::slotConnected() {
    m_tcpInput->handleConnect();
}

void TcpInputSession::handleRead()
{
    QTcpSocket* pSocket = static_cast<TcpClientInputFeed*>(
                m_pInput)->getSocket();
    QVector<ModesData> messages;
    for (;;) {
        if (pSocket->bytesAvailable() > 0) {
            qint64 nBytes = pSocket->bytesAvailable();
            if (nBytes > 0) {
                messages.clear();
                unsigned nMsg = m_pDecoder->tryDecode(pSocket->readAll(),
                                                      messages);
                m_stats.incBytes(nBytes);
                if (nMsg > 0) {
                    m_stats.incMessages(nMsg);
                    if (m_pDb) {
                        int nMsg = messages.size();
                        while (nMsg)
                            nMsg -= m_pDb->addMessages(messages);
                    }
                }
                emit statsUpdated(m_stats);
            }
        }
        else
            break;
    }
}
