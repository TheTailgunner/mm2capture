#include "tcp-client.h"
#include <QAbstractSocket>
#include <QByteArray>
#include <QDataStream>
#include <stdexcept>

using namespace MM2Capture;

TcpClientImpl::TcpClientImpl(): m_nPort{0}, m_isRunning{false} {
}

TcpClientImpl::TcpClientImpl(const TcpClientImpl &obj)
{
    m_strHost = obj.m_strHost;
    m_nPort = obj.m_nPort;
    m_isRunning = false;
}

void
TcpClientImpl::start() {
    m_socket.connectToHost(m_strHost, m_nPort);
    if (!m_socket.waitForConnected(CONNECT_TIMEOUT_MSEC)) {
        throw std::runtime_error(m_socket.errorString().
                                 toStdString());
    }
    m_isRunning =
            m_socket.state() == QAbstractSocket::ConnectedState;
}

void
TcpClientImpl::stop() {
    m_socket.close();
    m_isRunning = false;
}

qint64
TcpClientImpl::read(QByteArray &in) {
    if (!m_isRunning)
        throw std::runtime_error("TcpClientImpl::read(): does not started");
    if (!m_socket.waitForReadyRead(-1))
        throw std::runtime_error(m_socket.errorString().
                                 toStdString());
    qint64 nBytesToRead = m_socket.bytesAvailable();
    if (nBytesToRead)
        in = std::move(m_socket.readAll());
    return nBytesToRead;
}

qint64 TcpClientImpl::write(const QByteArray &in) {
    if (!m_isRunning)
        throw std::runtime_error("TcpClientImpl::read(): does not started");
    m_socket.write(in);
    if (!m_socket.waitForBytesWritten(-1)) {
        throw std::runtime_error(m_socket.errorString().
                                 toStdString());
    }
    return in.length();
}

TcpClientImpl::~TcpClientImpl() {
    stop();
}
