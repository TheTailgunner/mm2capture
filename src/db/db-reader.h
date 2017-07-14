/// \file DB reader implementation pulls chunks from selected session
#ifndef _DB_READER_H_
#define _DB_READER_H_

#include <QtSql>
#include <QtGlobal>
#include <QSharedPointer>

class QByteArray;

namespace MM2Capture {

class DBReader {
public:
    using Ptr = QSharedPointer<DBReader>;
    DBReader();
    explicit DBReader(const QString &strFile);
    void open();
    void setFile(const QString &strFn) {
        m_dbFilename = strFn;
    }
    bool isOpen() const {
        return m_isOpen;
    }
    void tryUseSession(quint64);
    bool getNextChunk(QByteArray &);
    void close();
    QSqlQuery getAvailableSessions() const;
    ~DBReader() {
        close();
    }
private:
    static const QString CONNECTION_NAME;
    bool m_isOpen;

    bool m_sessionSelected;
    quint64 m_sessionId;

    QString m_dbFilename;
    QSqlDatabase m_dbHandler;
    QSqlQuery m_sessionData;
};

}
#endif
