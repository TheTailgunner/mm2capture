#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <QObject>
#include "db/db-reader.h"
#include <QSqlQueryModel>

namespace MM2Capture {

class Player: public QObject {
    Q_OBJECT
public:
    Player(QObject *prnt = nullptr);
    void setReader(const DBReader::Ptr pr) {
        m_pDbReader = pr;
    }
    void start();
    void stop();
private:
    bool m_isRunning;
    DBReader::Ptr m_pDbReader;
};

}

#endif
