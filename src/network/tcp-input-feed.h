#ifndef _TCP_INPUT_FEED_H
#define _TCP_INPUT_FEED_H

#include <QtNetwork/QTcpSocket>

#include "base-input-feed.h"

namespace MM2Capture {

class TcpClientInputFeed;

class SocketSignalMapper: public QObject {
    Q_OBJECT
public:
    SocketSignalMapper(TcpClientInputFeed *pFeed): m_tcpInput{pFeed} {
    }
public slots:
    void slotConnected();
    void slotError(QAbstractSocket::SocketError e) {
        int dummy = 42;
    }

private:
    TcpClientInputFeed *m_tcpInput{nullptr};
};

class TcpInputSession: public BaseInputSession {
    Q_OBJECT
public:
    TcpInputSession(const BaseInputFeed::Ptr &feed):
        BaseInputSession{feed} {
    }
private:
    void handleRead();
};

class TcpClientInputFeed: public BaseInputFeed {
public:
    TcpClientInputFeed();
    TcpClientInputFeed(const TcpClientInputFeed &);
    bool operator==(const TcpClientInputFeed &);
    TcpClientInputFeed& operator=(const TcpClientInputFeed &);

    void setHost(const QString& host) {
        m_strHost = host;
    }

    void setPort(const quint16 port) {
        m_nPort = port;
    }

    QTcpSocket* getSocket() {
        return m_pSocket.data();
    }

    virtual ~TcpClientInputFeed() {
        m_pSocket->disconnect();
        m_pSession->disconnect();
        m_pMapper->disconnect();
        implStop();
    }

private:
    friend class SocketSignalMapper;
    void implStart();
    void implStop();
    void handleConnect();

    QScopedPointer<QTcpSocket> m_pSocket;
    BaseInputSession::Ptr m_pSession;
    QString m_strHost;
    quint16 m_nPort;
    QScopedPointer<SocketSignalMapper> m_pMapper;
};

}
#endif
