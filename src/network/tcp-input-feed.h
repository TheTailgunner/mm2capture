#ifndef _TCP_INPUT_FEED_H
#define _TCP_INPUT_FEED_H

#include <QVector>

#include "abstract-feed.h"
#include "io-impl/tcp-client.h"
#include "../modes/modes-data.h"

namespace MM2Capture {

class TcpClientInputFeed: public AbstractInputFeed,
        protected TcpClientImpl {
public:
    TcpClientInputFeed();
    void start();
    operator bool() const {
        return m_dataRead;
    }
    void setHost(const QString &strHost) {
        TcpClientImpl::setHost(strHost);
    }
    void setPort(quint16 nPort) {
        TcpClientImpl::setPort(nPort);
    }
    AbstractInputFeed &operator >>(QVector<ModesData> &out);
    void stop();
    ~TcpClientInputFeed();
private:
    bool m_isRunning;
    bool m_dataRead;
};

}
#endif
