#include "db-reader.h"
#include "db-exception.h"
#include "db-chunk.h"

#include <QFile>
#include <QByteArray>

using namespace MM2Capture;

const QString DBReader::CONNECTION_NAME = "reader";

DBReader::DBReader(): m_isOpen{false}, m_sessionSelected{false}
{ }

DBReader::DBReader(const QString &strFile): m_isOpen{false},
    m_sessionSelected{false}, m_dbFilename{strFile}
{ }

void
DBReader::open() {
    if (m_isOpen)
        return;
    if (m_dbFilename.isEmpty())
        throw std::runtime_error("DBReader::open(): no filename specified");
    if (!QFile::exists(m_dbFilename))
        throw std::runtime_error("DBReader::open():"
                                 + m_dbFilename.toStdString() +
                                 " is not exists");
    m_dbHandler = QSqlDatabase::addDatabase("QSQLITE",
                                                CONNECTION_NAME);
    m_dbHandler.setDatabaseName(m_dbFilename);
    m_isOpen = m_dbHandler.open();
    if (!m_isOpen) {
        QSqlError err = m_dbHandler.lastError();
        m_dbHandler = QSqlDatabase();
        QSqlDatabase::removeDatabase(CONNECTION_NAME);
        throw DBException(err);
    }
}

void
DBReader::tryUseSession(quint64 sessionId)
{
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
    m_sessionData = query;
    m_sessionSelected = true;
    m_sessionId = sessionId;
}

bool
DBReader::getNextChunk(QByteArray &outChunk) {
    if (!m_sessionSelected)
        throw std::runtime_error("DBReader::getNextChunk(): no session"
                                 "selected");
    if (m_sessionData.next()) {
        outChunk = std::move(m_sessionData.value(0).toByteArray());
        return true;
    }
    return false;
}

void
DBReader::close() {
    if (!m_isOpen)
        return;
    m_dbHandler.close();
    m_dbHandler = QSqlDatabase();
    QSqlDatabase::removeDatabase(CONNECTION_NAME);
    m_isOpen = m_sessionSelected = false;
}

QSqlQuery
DBReader::getAvailableSessions() const
{
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
