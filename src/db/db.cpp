#include "db.h"

#include <QFile>

using namespace MM2Capture;

const QString DBWriter::CONNECTION_NAME = "write";

DBWriter::DBWriter(const QString &path): m_dbPath{path},
    m_isOpened{false}, m_isSessionOpen{false}, m_chunkNumber{0} {
}

void
DBWriter::open(const QString& sessionName) {
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

unsigned
DBWriter::addMessages(QVector<ModesData> &inVec)
{
    if (m_currentChunk.isValid()) {
        flushChunk(false);
    }
    unsigned count = m_currentChunk.addMessages(inVec);
    return count;
}

void
DBWriter::close() {
    QSqlError err;
    closeSession(err);
    try {
        flushChunk(true);
    } catch (const DBException&) {
        // logging
    }

    QSqlDatabase::database(CONNECTION_NAME).close();
    QSqlDatabase::removeDatabase(CONNECTION_NAME);
}

QSqlError
DBWriter::prepareTables(QSqlDatabase &db) {
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

bool
DBWriter::openSession(QSqlDatabase &db, const QString &sessionName,
                      QSqlError &outErr)
{
    QSqlQuery query("INSERT INTO input_sessions(startTimestamp, name) "
                    "VALUES ("
                    "DATETIME('now', 'localtime'), '" + sessionName +"')", db);
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

void
DBWriter::flushChunk(bool flush)
{
    if (!m_isOpened)
        throw std::runtime_error("flushChunk(): connection is not open");
    if (!m_isSessionOpen)
        throw std::runtime_error("flushChunk(): session is not open");
    QSqlQuery query("", QSqlDatabase::database(CONNECTION_NAME));
    QString strQuery = "INSERT INTO chunks(sessionID, startTimestamp, chunkNumber, chunkData) VALUES("
                    ":SID, :STS, :CN, :CD)";
    query.prepare(strQuery);
    query.bindValue(":SID", m_sessionId);
    query.bindValue(":STS", m_currentChunk.startTime());
    query.bindValue(":CN", m_chunkNumber);
    if (flush) {
        QByteArray chunk;
        if (m_currentChunk.flush(chunk))
            query.bindValue(":CD", chunk);
    }
    else
        query.bindValue(":CD", m_currentChunk.compressed());
    query.exec();
    QSqlError err = query.lastError();
    if (err.type() != QSqlError::NoError)
        throw DBException(err);
    ++m_chunkNumber;
    m_currentChunk.clear();
}

bool
DBWriter::closeSession(QSqlError &outErr)
{
    if (!m_isOpened || !m_isSessionOpen)
        return true;
    QSqlQuery query("", QSqlDatabase::database(CONNECTION_NAME));
    query.prepare("UPDATE input_sessions SET endTimestamp = DATETIME('now', 'localtime') WHERE id = :SID");
    query.bindValue(":SID", m_sessionId);
    query.exec();
    outErr = query.lastError();
    return outErr.type() == QSqlError::NoError;
}
