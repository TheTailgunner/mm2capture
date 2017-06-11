/*! \file SQLite DB facilities implemention
 */
#ifndef _DB_H
#define _DB_H

#include <QtSql>
#include <QString>
#include <QSharedPointer>
#include <QVector>
#include <stdexcept>

#include "../modes/modes-data.h"
#include "db-chunk.h"

namespace MM2Capture {

/*! \brief DB exception.
 * \details Contains native QtSql error object (QSqlError).
 */
struct DBException: public std::runtime_error {
public:
    //! Creates eception object with appoprite QSqlError
    DBException(const QSqlError &err):
        std::runtime_error(err.text().toStdString()),
        m_errObj{err} {
    }
    //! Returns QSqlError description
    virtual const char *what() const throw() {
        return m_errObj.text().toStdString().c_str();
    }
    //! Returns native underlying QSqlError object
    inline const QSqlError &getSqlError() const {
        return m_errObj;
    }
private:
    QSqlError m_errObj;
};

class DBWriter {
public:
    using Ptr = QSharedPointer<DBWriter>;
    DBWriter();
    explicit DBWriter(const QString &strFilename);
    void setFilename(const QString &strFn) {
        m_dbPath = strFn;
    }
    void open(const QString &sessionName);
    inline bool isOpened() const {
        return m_isOpened;
    }
    unsigned addMessages(QVector<ModesData> &);
    void close();
    ~DBWriter() {
        close();
    }
private:
    static const QString CONNECTION_NAME;
    QString m_dbPath;
    QString m_sessionName;
    bool m_isOpened;
    bool m_isSessionOpen;
    quint64 m_sessionId;
    DBChunk m_currentChunk;
    quint64 m_chunkNumber;

    QSqlError prepareTables(QSqlDatabase &);
    bool openSession(QSqlDatabase &, const QString &, QSqlError &);
    void flushChunk(bool);
    bool closeSession(QSqlError &);
};

}
#endif
