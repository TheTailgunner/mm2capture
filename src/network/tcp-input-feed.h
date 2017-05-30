#ifndef _TCP_INPUT_FEED_H
#define _TCP_INPUT_FEED_H

#include <QtNetwork/QTcpSocket>

#include "base-input-feed.h"
#include "feed-counter.h"
#include "../modes/modes-decoder.h"

namespace MM2Capture {

class TcpClientInputFeed;
class ModesDecoder;

class SocketSignalMapper: public QObject {
    Q_OBJECT
public:
    SocketSignalMapper(TcpClientInputFeed *pFeed): m_tcpInput{pFeed} {
    }
public slots:
    void slotConnected();
    void slotError() {
    }

private:
    TcpClientInputFeed *m_tcpInput{nullptr};
};

class TcpInputSession: public BaseInputSession {
    Q_OBJECT
public:
    TcpInputSession(BaseInputFeed* feed):
        BaseInputSession{feed}, m_pDecoder{new ModesDecoder()} {
    }
private:
    void handleRead();
    QScopedPointer<ModesDecoder> m_pDecoder;
    FeedCounter m_stats;
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

    void setPort(quint16 port) {
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
    void generateIdent();
    void handleConnect();

    QScopedPointer<QTcpSocket> m_pSocket;
    QString m_strHost;
    quint16 m_nPort;
    QScopedPointer<SocketSignalMapper> m_pMapper;
};

}
#endif
