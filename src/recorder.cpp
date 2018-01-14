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

#include "recorder.h"
#include <stdexcept>
#include <QVector>
#include "modes/modes-data.h"
#include "network/feed-counter.h"
#include "network/tcp-input-feed.h"

using namespace MM2Capture;

Recorder::Recorder(QObject *prnt)
    : QThread(prnt), m_pDbWriter{new DBWriter()} {}

void Recorder::startWork() {
  if (m_strOutFile.isEmpty())
    emit error("No DB filename");
  if (!m_pInputStream)
    emit error("No input stream");
  m_pDbWriter->setFilename(m_strOutFile);
  m_pInputStream->start();
  m_pDbWriter->open(m_pInputStream->id());
}

void Recorder::stopWork() {
  m_pDbWriter->close();
  m_pInputStream->stop();
}

void Recorder::run() {
  try {
    startWork();
  } catch (const std::runtime_error &exc) {
    emit error(QString(exc.what()));
    stopWork();
    return;
  }
  QVector<ModesData> msgs;
  while (true) {
    if (isInterruptionRequested()) {
      stopWork();
      return;
    }
    if (*m_pInputStream >> msgs) {
      unsigned nMsgs = msgs.size();
      while (nMsgs) {
        nMsgs -= m_pDbWriter->addMessages(msgs);
      }
      emit networkStatsUpdated(m_pInputStream->stats());
    }
  }
  stopWork();
}
