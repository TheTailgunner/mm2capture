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

struct DBException: public std::runtime_error {
public:
    DBException(const QSqlError &err):
        std::runtime_error(err.text().toStdString()),
        m_errObj{err} {
    }
    virtual const char *what() const throw() {
        return m_errObj.text().toStdString().c_str();
    }

    inline const QSqlError &getSqlError() const {
        return m_errObj;
    }
private:
    QSqlError m_errObj;
};

class DBWriter {
public:
    using Ptr = QSharedPointer<DBWriter>;
    explicit DBWriter(const QString &);
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
