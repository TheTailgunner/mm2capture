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

/// \file DB reader implementation pulls chunks from selected session
#ifndef _DB_READER_H_
#define _DB_READER_H_

#include <QtSql>
#include <QtGlobal>
#include <QSharedPointer>
#include "db-session-stats.h"

class QByteArray;

namespace MM2Capture {

class DBReader {
public:
  using Ptr = QSharedPointer<DBReader>;
  DBReader();
  explicit DBReader(const QString &strFile);
  void open();
  void setFile(const QString &strFn) { m_dbFilename = strFn; }
  bool isOpen() const { return m_isOpen; }
  void tryUseSession(quint64, DBSessionStats &);
  bool getNextChunk(QByteArray &);
  void close();
  QSqlQuery getAvailableSessions() const;
  ~DBReader() { close(); }

private:
  static const QString CONNECTION_NAME;

  void loadSessionStats(quint64, DBSessionStats &);

  bool m_isOpen;

  bool m_sessionSelected;
  quint64 m_sessionId;

  QString m_dbFilename;
  QSqlDatabase m_dbHandler;
  QSqlQuery m_sessionData;
};
}
#endif
