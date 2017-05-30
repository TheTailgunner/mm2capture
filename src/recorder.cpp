#include "recorder.h"

using namespace MM2Capture;

Recorder::Recorder(const QString &dbPath, const BaseInputFeed::Ptr &feed, QObject *parent) : QObject(parent),
    m_pWriter{new DBWriter(dbPath)}, m_pFeed{feed}, m_isRunning{false}
{ }

void Recorder::start()
{
    if (m_isRunning)
        return;
    m_pWriter->open(m_pFeed->getIdent());
    m_pFeed->start(m_pWriter);
    m_isRunning = true;
    QObject::connect(m_pFeed->getSession().data(),
                     SIGNAL(statsUpdated(FeedCounter)),
                     this, SLOT(slotStatsUpdated(FeedCounter)));
}

void
Recorder::stop()
{
    if (!m_isRunning)
         return;
     m_pFeed->stop();
     m_pWriter->close();
}

void
Recorder::slotStatsUpdated(const FeedCounter &stats) {
    emit statsUpdated(stats);
}
