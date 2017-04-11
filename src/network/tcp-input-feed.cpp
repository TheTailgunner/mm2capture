#include "tcp-input-feed.h"

using namespace MM2Capture;

TcpClientInputFeed::TcpClientInputFeed():
    m_pSocket{new QTcpSocket(nullptr)}, m_pMapper{new SocketSignalMapper(this)} {
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
    TcpClientInputFeed tmp{rhs};
    qSwap(*this, tmp);
    return *this;
}

void
TcpClientInputFeed::implStart() {
    QObject::connect(m_pSocket.data(), SIGNAL(error(QAbstractSocket::SocketError)),
                     m_pMapper.data(), SLOT(slotError(QAbstractSocket::SocketError)));
    m_pSocket->connectToHost(m_strHost, m_nPort);
    QObject::connect(m_pSocket.data(), SIGNAL(connected()),
                     m_pMapper.data(), SLOT(slotConnected()));
}

void
TcpClientInputFeed::implStop() {
    m_pSocket->disconnectFromHost();
}

void TcpClientInputFeed::handleConnect()
{
    m_pSession = BaseInputSession::Ptr(new TcpInputSession(sharedFromThis()));
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
                m_pInput.data())->getSocket();
    QByteArray bytes;
    for (;;) {
        if (pSocket->bytesAvailable() > 0) {
            bytes.clear();
            bytes.append(pSocket->readAll());
        }
        else
            break;
    }
}
