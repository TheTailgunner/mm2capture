#include "src/network/feed-factory.h"

#include <QSharedPointer>
#include "tcp-input-feed.h"

using namespace MM2Capture;

BaseInputFeed::Ptr
FeedFactory::createTcpClientInput(const QString &strHost, quint16 nPort)
{
    BaseInputFeed::Ptr pFeed(new TcpClientInputFeed());
    TcpClientInputFeed* pTcpFeed =
            qSharedPointerCast<TcpClientInputFeed>(pFeed).data();
    pTcpFeed->setHost(strHost);
    pTcpFeed->setPort(nPort);
    pFeed->generateIdent();
    return pFeed;
}
