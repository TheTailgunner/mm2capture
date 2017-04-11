#include "mainwindow.h"
#include "network/base-input-feed.h"
#include "network/tcp-input-feed.h"
#include <QApplication>

using namespace MM2Capture;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    QSharedPointer<TcpClientInputFeed> pFeed(new TcpClientInputFeed());
    pFeed->setHost("127");
    pFeed->setPort(44044);
    pFeed->start();
    return a.exec();
}
