#include "db-writer.h"
#include "db-exception.h"
#include <compress/packer.h>

#include <QFile>
#include <algorithm>
#include <QDataStream>
#include <stdexcept>

using namespace MM2Capture;

const QString DBWriter::CONNECTION_NAME = "write";

DBWriter::DBWriter()
    : m_isOpened{false}, m_isSessionOpen{false}, m_chunkNumber{0} {}

DBWriter::DBWriter(const QString &strFilename)
    : m_dbPath{strFilename}, m_isOpened{false}, m_isSessionOpen{false},
      m_chunkNumber{0} {}

void DBWriter::open(const QString &sessionName) {
  if (m_dbPath.isEmpty())
    throw std::runtime_error("DBWriter::open(): wrong DB filename");
  bool fileExists = QFile::exists(m_dbPath);
  QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", CONNECTION_NAME);
  db.setDatabaseName(m_dbPath);
  QSqlError err;
  m_isOpened = db.open();
  if (!m_isOpened ||
      (!fileExists && prepareTables(db).type() != QSqlError::NoError)) {
    err = db.lastError();
    db = QSqlDatabase(); // prevents dangling queries
    QSqlDatabase::removeDatabase(CONNECTION_NAME);
    throw DBException(err);
  }

  if (!openSession(db, sessionName, err)) {
    close();
    throw DBException(err);
  }
}

unsigned DBWriter::addMessages(QVector<ModesData> &msgs) {
  if (!m_isSessionOpen || !m_isOpened)
    throw std::runtime_error("DBWriter::addMessages: connection is not open");
  unsigned nResult = 0;
  int spaceAvail = MAX_MESSAGES_TO_FLUSH - m_msgsToFlush.size();
  int nInMsgs = spaceAvail;
  if (msgs.size() <= spaceAvail)
    nInMsgs = msgs.size();

  if (nInMsgs > 0)
    std::copy_n(msgs.begin(), nInMsgs, std::back_inserter(m_msgsToFlush));
  nResult += nInMsgs;

  if (m_msgsToFlush.size() == MAX_MESSAGES_TO_FLUSH) {
    try {
      flushChunk();
    } catch (const std::runtime_error &exc) {
      (void)exc;
      // erase passed messages from the flush buffer on error -
      // restoring passed message buffer
      m_msgsToFlush.erase(m_msgsToFlush.begin() + nInMsgs, m_msgsToFlush.end());
      // TODO log output
      return 0;
    }
  }

  msgs.erase(msgs.begin(), msgs.begin() + nInMsgs);

  if (msgs.size() > 0) {
    spaceAvail = MAX_MESSAGES_TO_FLUSH;
    nInMsgs = spaceAvail;
    if (msgs.size() <= spaceAvail)
      nInMsgs = msgs.size();
    if (nInMsgs > 0) {
      std::copy_n(msgs.begin(), nInMsgs, std::back_inserter(m_msgsToFlush));
      msgs.erase(msgs.begin(), msgs.begin() + nInMsgs);
    }
    nResult += nInMsgs;
  }
  return nResult;
}

void DBWriter::close() {
  if (!m_isOpened)
    return;
  QSqlError err;
  try {
    flushChunk();
  } catch (const std::runtime_error &exc) {
    qDebug() << exc.what();
  }
  // TODO logging if closeSession() fails
  if (m_isSessionOpen)
    closeSession(err);
  QSqlDatabase::database(CONNECTION_NAME).close();
  QSqlDatabase::removeDatabase(CONNECTION_NAME);
  m_isOpened = m_isSessionOpen = false;
}

QSqlError DBWriter::prepareTables(QSqlDatabase &db) {
  QSqlQuery query("", db);
  query.exec("CREATE TABLE chunks ("
             "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
             "sessionId INTEGER NOT NULL,"
             "startTimestamp INTEGER NOT NULL,"
             "chunkNumber INTEGER NOT NULL,"
             "chunkData BLOB NOT NULL)");
  query.exec("CREATE TABLE input_sessions ("
             "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
             "startTimestamp INTEGER NOT NULL,"
             "endTimestamp INTEGER,"
             "name TEXT NOT NULL)");
  return query.lastError();
}

bool DBWriter::openSession(QSqlDatabase &db, const QString &sessionName,
                           QSqlError &outErr) {
  QSqlQuery query("INSERT INTO input_sessions(startTimestamp, name) "
                  "VALUES ("
                  "DATETIME('now', 'localtime'), '" +
                      sessionName + "')",
                  db);
  if (query.lastError().type() == QSqlError::NoError) {
    QVariant lastInsertId = query.lastInsertId();
    if (lastInsertId.isValid()) {
      m_sessionId = lastInsertId.toULongLong();
      m_isSessionOpen = true;
      m_sessionName = sessionName;
      return true;
    }
  }
  outErr = query.lastError();
  return false;
}

void DBWriter::flushChunk() {
  if (m_msgsToFlush.size() == 0)
    return;
  QSqlQuery query("", QSqlDatabase::database(CONNECTION_NAME));
  QString strQuery = "INSERT INTO chunks(sessionId, startTimestamp, "
                     "chunkNumber, chunkData) VALUES("
                     ":SID, :STS, :CN, :CD)";
  query.prepare(strQuery);
  query.bindValue(":SID", m_sessionId);
  query.bindValue(":STS", m_msgsToFlush[0].timestamp()); // timestamp of the
                                                         // chunk is the
                                                         // timestamp of first
                                                         // message in current
                                                         // one
  query.bindValue(":CN", m_chunkNumber);

  QByteArray serializedMessagesData;
  QDataStream strm(&serializedMessagesData, QIODevice::WriteOnly);
  strm.setVersion(QDataStream::Qt_4_0);
  strm << m_msgsToFlush;
  m_msgsToFlush.clear();

  QByteArray deflatedData = Packer::compress(serializedMessagesData);
  query.bindValue(":CD", deflatedData);

  query.exec();
  QSqlError err = query.lastError();
  if (err.type() != QSqlError::NoError)
    throw DBException(err);
  ++m_chunkNumber;
}

bool DBWriter::closeSession(QSqlError &outErr) {
  if (!m_isOpened || !m_isSessionOpen)
    return true;
  QSqlQuery query("", QSqlDatabase::database(CONNECTION_NAME));
  query.prepare("UPDATE input_sessions SET endTimestamp = DATETIME('now', "
                "'localtime') WHERE id = :SID");
  query.bindValue(":SID", m_sessionId);
  query.exec();
  outErr = query.lastError();
  return outErr.type() == QSqlError::NoError;
}
