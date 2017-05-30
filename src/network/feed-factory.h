#ifndef FEEDFACTORY_H
#define FEEDFACTORY_H

#include "base-input-feed.h"

namespace MM2Capture {

class FeedFactory
{
public:
    FeedFactory() =delete;
    FeedFactory(const FeedFactory &) =delete;
    FeedFactory& operator==(const FeedFactory &) =delete;
    ~FeedFactory() =delete;

    static BaseInputFeed::Ptr createTcpClientInput(const QString&, quint16);
};

}
#endif // FEEDFACTORY_H
