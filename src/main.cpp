#include "mainwindow.h"
#include "network/tcp-input-feed.h"
#include <QApplication>
#include <QByteArray>
#include <stdexcept>
#include <QDebug>
#include <QVector>
#include <QTimer>

using namespace MM2Capture;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
