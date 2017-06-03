#include "src/network/feed-factory.h"

#include <QSharedPointer>
#include "tcp-input-feed.h"

using namespace MM2Capture;

BaseInputFeed::Ptr
FeedFactory::createTcpClientInput(const QString &strHost, quint16 nPort)
{
    return BaseInputFeed::Ptr();
}
