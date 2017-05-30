#ifndef RECORDER_H
#define RECORDER_H

#include <QObject>
#include "db/db.h"
#include "network/base-input-feed.h"
#include "network/tcp-input-feed.h"
#include "network/feed-counter.h"

namespace MM2Capture {

class Recorder : public QObject
{
    Q_OBJECT
public:
    explicit Recorder(const QString &, const BaseInputFeed::Ptr& ,
                      QObject * = 0);
    void start();
    void stop();
    ~Recorder() {
        if (m_isRunning)
            stop();
    }

signals:
    void statsUpdated(const FeedCounter &);
private slots:
    void slotStatsUpdated(const FeedCounter &);
private:
    DBWriter::Ptr m_pWriter;
    BaseInputFeed::Ptr m_pFeed;
    bool m_isRunning;
};

}
#endif // RECORDER_H
