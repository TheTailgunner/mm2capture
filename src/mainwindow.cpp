#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "network/tcp-input-feed.h"

#include <QMessageBox>
#include <QThread>
#include <stdexcept>
#include <QIntValidator>
#include <QFileDialog>
//#include <QtGlobal>

using namespace MM2Capture;

QString formatBytesCount(unsigned long nBytes) {
    static const char* SUFFIXES[] = {
        "Byte(s)",
        "Kb",
        "Mb",
        "Gb"
    };

    unsigned iSuffix = 0;
    unsigned long nb = nBytes;
    while (nb / 1024) {
        nb /= 1024;
        ++iSuffix;
    }
    QString res = QString("%1").arg(nb) + " " + SUFFIXES[iSuffix];
    return res;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_pRecorder{new Recorder()}
{
    ui->setupUi(this);
    ui->portEdit->setValidator(new QIntValidator(0, 1 << 16, this));
    QObject::connect(ui->startRecordButton, SIGNAL(clicked(bool)), this, SLOT(slotRecordStart()));
    QObject::connect(ui->stopRecordButton, SIGNAL(clicked(bool)), this, SLOT(slotRecordStop()));
    QObject::connect(m_pRecorder, SIGNAL(started()), this, SLOT(slotRecorderStarted()));
    QObject::connect(m_pRecorder, SIGNAL(networkStatsUpdated(FeedCounter)),
                     this, SLOT(slotUpdateRecordStats(FeedCounter)), Qt::DirectConnection);
    QObject::connect(m_pRecorder, SIGNAL(error(QString)), this, SLOT(slotRecordError(QString)));
    QObject::connect(m_pRecorder, SIGNAL(finished()), this, SLOT(slotRecorderFinished()));
    QObject::connect(ui->selectFileButton, SIGNAL(clicked(bool)), this,
                     SLOT(slotSelectFile()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void
MainWindow::slotRecordStart()
{
    if (ui->fileNameEdit->text().isEmpty()) {
        QMessageBox::critical(this, "Error",
                              "No output filename",
                              QMessageBox::Ok);
        return;
    }
    if (ui->addressEdit->text().isEmpty() ||
            ui->portEdit->text().isEmpty()) {
        QMessageBox::critical(this, "Error",
                              "No host/port to connect",
                              QMessageBox::Ok);
        return;
    }
    AbstractInputFeed::Ptr pFeed(new TcpClientInputFeed(ui->addressEdit->text(),
                                                        ui->portEdit->text().toInt()));
    m_pRecorder->setInput(pFeed);
    m_pRecorder->setFilename(ui->fileNameEdit->text());
    m_pRecorder->start();
}

void
MainWindow::slotRecordStop() {
    m_pRecorder->requestInterruption();
    m_pRecorder->wait();;
}

void
MainWindow::slotUpdateRecordStats(const FeedCounter &stats)
{
    ui->bytesCountLabel->setText(formatBytesCount(
                                     stats.getBytes())
                                 );
    ui->msgsCountLabel->setText(QString("%1").arg(
                                    stats.getMessages()
                                    ));
}

void MainWindow::slotRecordError(const QString &strDescr)
{
    QMessageBox::critical(0, "Recorder error",
                          strDescr,
                          QMessageBox::Ok);
    qDebug() << strDescr;
}

void
MainWindow::slotRecorderStarted()
{
    ui->fileNameEdit->setEnabled(false);
    ui->selectFileButton->setEnabled(false);
    ui->addressEdit->setEnabled(false);
    ui->portEdit->setEnabled(false);
    ui->startRecordButton->setEnabled(false);
    ui->stopRecordButton->setEnabled(true);
    setWindowTitle("[REC] MM2Capture");
}

void MainWindow::slotRecorderFinished()
{
    ui->fileNameEdit->setEnabled(true);
    ui->selectFileButton->setEnabled(true);
    ui->addressEdit->setEnabled(true);
    ui->portEdit->setEnabled(true);
    ui->startRecordButton->setEnabled(true);
    ui->stopRecordButton->setEnabled(false);
    setWindowTitle("MM2Capture");
}

void
MainWindow::slotSelectFile() {
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    "Save DB",
                                                    "", "SQLite3 DB(*.db);");
    if (!fileName.isEmpty())
        ui->fileNameEdit->setText(fileName);
}
