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

#include "db-reader.h"
#include "db-exception.h"
#include "db-chunk.h"

#include <QFile>
#include <QByteArray>
#include <QDateTime>

using namespace MM2Capture;

const QString DBReader::CONNECTION_NAME = "reader";

DBReader::DBReader() : m_isOpen{false}, m_sessionSelected{false} {}

DBReader::DBReader(const QString &strFile)
    : m_isOpen{false}, m_sessionSelected{false}, m_dbFilename{strFile} {}

void DBReader::open() {
  if (m_isOpen)
    return;
  if (m_dbFilename.isEmpty())
    throw std::runtime_error("DBReader::open(): no filename specified");
  if (!QFile::exists(m_dbFilename))
    throw std::runtime_error("DBReader::open():" + m_dbFilename.toStdString() +
                             " is not exists");
  m_dbHandler = QSqlDatabase::addDatabase("QSQLITE", CONNECTION_NAME);
  m_dbHandler.setDatabaseName(m_dbFilename);
  m_isOpen = m_dbHandler.open();
  if (!m_isOpen) {
    QSqlError err = m_dbHandler.lastError();
    m_dbHandler = QSqlDatabase();
    QSqlDatabase::removeDatabase(CONNECTION_NAME);
    throw DBException(err);
  }
}

void DBReader::tryUseSession(quint64 sessionId,
                             DBSessionStats &outSessionInfo) {
  if (!m_isOpen || m_sessionSelected)
    return;
  QSqlQuery query("", m_dbHandler);
  QString strQuery = QString("SELECT chunkData FROM"
                             " chunks WHERE sessionId = %1"
                             " ORDER BY chunkNumber;").arg(sessionId);
  query.prepare(strQuery);
  if (!query.exec()) {
    QSqlError err = query.lastError();
    throw DBException(err);
  }
  loadSessionStats(sessionId, outSessionInfo);
  m_sessionData = query;
  m_sessionSelected = true;
  m_sessionId = sessionId;
}

bool DBReader::getNextChunk(QByteArray &outChunk) {
  if (!m_sessionSelected)
    throw std::runtime_error("DBReader::getNextChunk(): no session"
                             "selected");
  if (m_sessionData.next()) {
    outChunk = std::move(m_sessionData.value(0).toByteArray());
    return true;
  }
  return false;
}

void DBReader::close() {
  if (!m_isOpen)
    return;
  m_dbHandler.close();
  m_dbHandler = QSqlDatabase();
  QSqlDatabase::removeDatabase(CONNECTION_NAME);
  m_isOpen = m_sessionSelected = false;
}

QSqlQuery DBReader::getAvailableSessions() const {
  if (!m_isOpen)
    throw std::runtime_error("DBReader:;getAvailableSessions(): "
                             "reader instance is not open");
  QSqlQuery query("", m_dbHandler);
  query.exec("SELECT startTimestamp, endTimestamp, name "
             "FROM input_sessions;");
  if (query.lastError().type() != QSqlError::NoError)
    throw DBException(query.lastError());
  return query;
}

void DBReader::loadSessionStats(quint64 sessionId, DBSessionStats &outStats) {
  const static QString FORMAT_STR = "yyyy-MM-dd HH:mm:ss";

  QSqlQuery query("", m_dbHandler);
  QString strQuery = QString("SELECT startTimestamp, endTimestamp"
                             " FROM input_sessions"
                             " WHERE id = :SID");
  query.prepare(strQuery);
  query.bindValue(":SID", sessionId);
  query.exec();
  if (query.lastError().type() != QSqlError::NoError)
    throw DBException(query.lastError());
  query.first();
  quint64 startTimestamp =
      QDateTime::fromString(query.value(0).toString(), FORMAT_STR)
          .toMSecsSinceEpoch();
  QVariant endTimestampValue = query.value(1);
  if (endTimestampValue.isNull())
    throw std::runtime_error("Incomplete session selected.");
  quint64 endTimestamp = QDateTime::fromString(endTimestampValue.toString(),
                                               FORMAT_STR).toMSecsSinceEpoch();
  outStats.m_durationMsecs = endTimestamp - startTimestamp;
}
