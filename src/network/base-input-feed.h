#ifndef _BASE_INPUT_FEED_H
#define _BASE_INPUT_FEED_H
#include <QtCore>
#include <QSharedPointer>
#include <QObject>
#include <QtNetwork/QTcpSocket>
#include <QEnableSharedFromThis>

#include "feed-counter.h"

namespace MM2Capture {

class BaseInputFeed: public QEnableSharedFromThis<BaseInputFeed> {
public:
    using Ptr = QSharedPointer<BaseInputFeed>;
    BaseInputFeed() =default;

    inline void start() {
        if (m_isRunning)
            return;
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

    void setIdent(const QString& id) {
        m_ident = id;
    }

    const QString& getIdent() const {
        return m_ident;
    }
private:
    virtual void implStart() = 0;
    virtual void implStop() = 0;

    QString m_ident;
    bool m_isRunning;
};

class BaseInputSession: public QObject {
    Q_OBJECT
public:
    using Ptr = QSharedPointer<BaseInputSession>;
    BaseInputSession() =default;
    BaseInputSession(const BaseInputFeed::Ptr&);
    void start();
    void stop();

    bool isRunning() const {
        return m_isRunning;
    }
public slots:
    inline void slotHandleRead() {
        handleRead();
    }
protected:
    BaseInputFeed::Ptr m_pInput;
private:
    virtual void handleRead() = 0;

    bool m_isRunning;
    FeedCounter m_stats;
};

}
#endif
