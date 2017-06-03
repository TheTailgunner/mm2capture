#include "tcp-input-feed.h"
#include <QByteArray>
#include <stdexcept>
#include "../modes/modes-data.h"
#include "../modes/modes-decoder.h"

using namespace MM2Capture;

TcpClientInputFeed::TcpClientInputFeed(): AbstractInputFeed(),
    TcpClientImpl(), m_isRunning{false}, m_dataRead{false} {
}

void
TcpClientInputFeed::start() {
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
            if (nBytes && m_pDecoder->tryDecode(data, out) > 0)
                m_dataRead = true;
        } catch (const std::runtime_error &exc) {
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
