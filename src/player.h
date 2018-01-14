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

#ifndef PLAYER_H_
#define PLAYER_H_

#include <QObject>
#include <db/db-reader.h>
#include <modes/modes-data.h>
#include <network/tcp-output-feed.h>

namespace MM2Capture {

class Player : public QThread {
  Q_OBJECT
public:
  Player(QObject *prnt = nullptr);

  void setReader(const DBReader::Ptr pr) { m_pDbReader = pr; }

  void setOutput(const AbstractOutputFeed::Ptr pf) { m_pOutput = pf; }
public slots:
  void setRate(double rate) { m_speedupRate = rate; }

signals:
  void error(const QString &);
  void recordEnd();

protected:
  void run() override;
private slots:
  void startWork();
  void stopWork();

private:
  DBReader::Ptr m_pDbReader;
  AbstractOutputFeed::Ptr m_pOutput;
  double m_speedupRate;

  void sendMessage(const ModesData &);
};
}

#endif // PLAYER_H
