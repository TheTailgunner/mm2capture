#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "network/feed-factory.h"

#include <QMessageBox>
#include <stdexcept>

using namespace MM2Capture;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_pRecorder{nullptr}
{
    ui->setupUi(this);
    QObject::connect(ui->startButton, SIGNAL(clicked(bool)), this, SLOT(slotStart()));
    QObject::connect(ui->stopButton, SIGNAL(clicked(bool)), this, SLOT(slotStop()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void
MainWindow::slotStart()
{
    if (!m_feed)
        m_feed = FeedFactory::createTcpClientInput("88.204.77.165", 35570);
    if (!m_pRecorder)
        m_pRecorder = new Recorder("out.db", m_feed, this);
    try {
        m_pRecorder->start();
    } catch (const std::runtime_error & exc) {
         QMessageBox::critical(0, "Error", QString(exc.what()));
    }
}

void
MainWindow::slotStop() {
    m_pRecorder->stop();
}
