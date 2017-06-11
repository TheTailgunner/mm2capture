#include "tcp-input-feed.h"
#include <QByteArray>
#include <stdexcept>
#include "../modes/modes-data.h"
#include "../modes/modes-decoder.h"

using namespace MM2Capture;

TcpClientInputFeed::TcpClientInputFeed(): AbstractInputFeed(),
    TcpClientImpl(), m_isRunning{false}, m_dataRead{false} {
}

TcpClientInputFeed::TcpClientInputFeed(const QString &strHost, quint16 nPort):
    AbstractInputFeed(), TcpClientImpl(), m_isRunning{false}, m_dataRead{false}
{
    TcpClientImpl::setHost(strHost);
    TcpClientImpl::setPort(nPort);
}

void
TcpClientInputFeed::start() {
    if (m_isRunning)
        return;
    TcpClientImpl::start();
    m_isRunning = true;
}

AbstractInputFeed &
TcpClientInputFeed::operator >>(QVector<ModesData> &out) {
    if (!m_isRunning)
        m_dataRead = false;
    else {
        QByteArray data;
        try {
            qint64 nBytes = TcpClientImpl::read(data);
            m_stats.incBytes(nBytes);
            unsigned nMsg = 0;
            if (nBytes && (nMsg = m_pDecoder->tryDecode(data, out)) > 0) {
                m_dataRead = true;
                m_stats.incMessages(nMsg);
            }
        } catch (const std::runtime_error &exc) {
            (void) exc;
            m_dataRead = false;
        }
    }
    return *this;
}

void
TcpClientInputFeed::stop() {
    TcpClientImpl::stop();
    m_isRunning = false;
    m_dataRead = false;
}

TcpClientInputFeed::~TcpClientInputFeed() {
    if (m_isRunning) {
        TcpClientImpl::stop();
        m_isRunning = false;
        m_dataRead = false;
    }
}
