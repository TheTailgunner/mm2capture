#ifndef PLAYER_H_
#define PLAYER_H_

#include <QObject>
#include "db/db-reader.h"
#include "modes/modes-data.h"

namespace MM2Capture {

class Player: public QThread {
    Q_OBJECT
public:
    Player(QObject *prnt = nullptr);
    void setReader(const DBReader::Ptr pr) {
        m_pDbReader = pr;
    }
signals:
    void error(const QString &);
protected:
    void run() override;
private slots:
    void startWork();
    void stopWork();
private:
    DBReader::Ptr m_pDbReader;

    void sendMessage(const ModesData &);
};

}

#endif // PLAYER_H
