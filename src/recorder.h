#ifndef RECORDER_H
#define RECORDER_H

#include <QObject>
#include <QThread>
#include <stdexcept>
#include "db/db-writer.h"
#include "network/abstract-feed.h"

namespace MM2Capture {

struct FeedCounter;

class Recorder: public QThread {
    Q_OBJECT
public:
    Recorder(QObject *prnt = nullptr);
    void setInput(const AbstractInputFeed::Ptr &pIn) {
        m_pInputStream = pIn;
    }
    void setFilename(const QString &strFn) {
        m_strOutFile = strFn;
    }
protected:
    void run();
private slots:
    void startWork();
    void stopWork();
signals:
    void error(const QString &);
    void networkStatsUpdated(const FeedCounter &);
private:
    AbstractInputFeed::Ptr m_pInputStream;
    DBWriter::Ptr m_pDbWriter;
    QString m_strOutFile;
};

}
#endif // RECORDER_H
