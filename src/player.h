#ifndef PLAYER_H_
#define PLAYER_H_

#include <QObject>
#include <db/db-reader.h>
#include <modes/modes-data.h>
#include <network/tcp-output-feed.h>

namespace MM2Capture {

class Player : public QThread {
  Q_OBJECT
public:
  Player(QObject *prnt = nullptr);

  void setReader(const DBReader::Ptr pr) { m_pDbReader = pr; }

  void setOutput(const AbstractOutputFeed::Ptr pf) { m_pOutput = pf; }
public slots:
  void setRate(double rate) { m_speedupRate = rate; }

signals:
  void error(const QString &);
  void recordEnd();

protected:
  void run() override;
private slots:
  void startWork();
  void stopWork();

private:
  DBReader::Ptr m_pDbReader;
  AbstractOutputFeed::Ptr m_pOutput;
  double m_speedupRate;

  void sendMessage(const ModesData &);
};
}

#endif // PLAYER_H
