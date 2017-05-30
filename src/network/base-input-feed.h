#ifndef _BASE_INPUT_FEED_H
#define _BASE_INPUT_FEED_H
#include <QtCore>
#include <QSharedPointer>
#include <QObject>
#include <QtNetwork/QTcpSocket>
#include <QEnableSharedFromThis>

#include "../db/db.h"
#include "feed-counter.h"

namespace MM2Capture {

class FeedFactory;

class BaseInputFeed;

class BaseInputSession: public QObject {
    Q_OBJECT
public:
    using Ptr = QSharedPointer<BaseInputSession>;
    BaseInputSession(): m_pInput{nullptr}, m_isRunning{false} {
    }
    BaseInputSession(BaseInputFeed*);
    void start();
    void stop();

    inline bool isRunning() const {
        return m_isRunning;
    }

    inline void setDBWriter(const DBWriter::Ptr &pDb) {
        m_pDb = pDb;
    }

    virtual ~BaseInputSession() {
        stop();
    }
signals:
    void statsUpdated(const FeedCounter &);
public slots:
    inline void slotHandleRead() {
        handleRead();
    }
protected:
    DBWriter::Ptr m_pDb;
    BaseInputFeed *m_pInput;
private:
    virtual void handleRead() = 0;

    bool m_isRunning;
};

class BaseInputFeed: public QEnableSharedFromThis<BaseInputFeed> {
public:
    using Ptr = QSharedPointer<BaseInputFeed>;
    BaseInputFeed(): m_isRunning{false} {
    }

    inline void start(const DBWriter::Ptr &pDb) {
        if (m_isRunning)
            return;
        m_pDb = pDb;
        implStart();
        m_isRunning = true;
    }

    inline void stop() {
        if (!m_isRunning)
            return;
        implStop();
        m_isRunning = false;
    }

    bool isRunning() {
        return m_isRunning;
    }

    const QString& getIdent() const {
        return m_ident;
    }

    inline void setDb(const DBWriter::Ptr &pDb) {
        m_pDb = pDb;
    }

    const BaseInputSession::Ptr& getSession() const {
        return m_pSession;
    }
protected:
    BaseInputFeed(const BaseInputSession::Ptr &pSession):
        m_pSession{pSession}, m_isRunning{false} {

    }

    DBWriter::Ptr m_pDb;
    BaseInputSession::Ptr m_pSession;

    void setIdent(const QString& id) {
        m_ident = id;
    }

private:
    friend class FeedFactory;
    virtual void implStart() = 0;
    virtual void implStop() = 0;
    virtual void generateIdent() = 0;

    QString m_ident;
    bool m_isRunning;
};

}
#endif
