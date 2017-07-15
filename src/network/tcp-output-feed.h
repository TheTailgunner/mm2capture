#ifndef _TCP_OUTPUT_FEED_H
#define _TCP_OUTPUT_FEED_H

#include "abstract-feed.h"
#include "io-impl/tcp-client.h"

namespace MM2Capture {

class ModesData;

class TcpClientOutputFeed: public AbstractOutputFeed,
        protected TcpClientImpl {
public:
    TcpClientOutputFeed() = default;
    TcpClientOutputFeed(const QString &,
                        quint16);
    ~TcpClientOutputFeed();

    void start() override;
    void stop() override;

    void setHost(const QString &strHost) {
        TcpClientImpl::setHost(strHost);
    }

    void setPort(quint16 nPort) {
        TcpClientImpl::setPort(nPort);
    }

    AbstractOutputFeed &operator <<(const ModesData &) override;
};

}

#endif
