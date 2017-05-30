#include "mainwindow.h"
#include "db/db.h"
#include "network/tcp-input-feed.h"
#include "network/feed-factory.h"
#include <QApplication>

using namespace MM2Capture;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
