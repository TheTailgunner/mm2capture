#include "mainwindow.h"
#include "network/tcp-input-feed.h"
#include <QApplication>
#include <QByteArray>
#include <stdexcept>
#include <QDebug>
#include <QVector>
#include "modes/modes-data.h"

using namespace MM2Capture;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QVector<ModesData> msgs;
    TcpClientInputFeed in;
    try {
        in.setHost("127.0.0.1");
        in.setPort(30008);
        in.start();
    } catch (const std::runtime_error &exc) {
        qDebug() << "err on start: " << exc.what();
    }
    while (true) {
        if (in >> msgs) {
            for (auto &msg : msgs) {
                qDebug() << msg.timestamp();
            }
        }
    }
    in.stop();
    return a.exec();
}
