/* By Vladislav Nikolaev nvladislav9 at gmail dot com

This file is part of mm2capture.

mm2capture is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

mm2capture is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details. */

#ifndef RECORDER_H
#define RECORDER_H

#include <QObject>
#include <QThread>
#include <stdexcept>
#include <db/db-writer.h>
#include <network/abstract-feed.h>

namespace MM2Capture {

struct FeedCounter;

class Recorder : public QThread {
  Q_OBJECT
public:
  Recorder(QObject *prnt = nullptr);
  void setInput(const AbstractInputFeed::Ptr &pIn) { m_pInputStream = pIn; }
  void setFilename(const QString &strFn) { m_strOutFile = strFn; }

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
