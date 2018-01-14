#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "network/tcp-input-feed.h"
#include "network/tcp-output-feed.h"
#include "outputsessiondialog.h"
#include "db/db-session-stats.h"

#include <QMessageBox>
#include <QThread>
#include <stdexcept>
#include <QIntValidator>
#include <QFileDialog>

using namespace MM2Capture;

QString formatBytesCount(unsigned long nBytes) {
  static const char *SUFFIXES[] = {"Byte(s)", "Kb", "Mb", "Gb"};

  unsigned iSuffix = 0;
  unsigned long nb = nBytes;
  while (nb / 1024) {
    nb /= 1024;
    ++iSuffix;
  }
  QString res = QString("%1").arg(nb) + " " + SUFFIXES[iSuffix];
  return res;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow),
      m_pRecorder{new Recorder(this)}, m_pPlayer{new Player(this)} {
  ui->setupUi(this);
  ui->inputPortEdit->setValidator(new QIntValidator(0, 1 << 16, this));
  QObject::connect(ui->startRecordButton, SIGNAL(clicked(bool)), this,
                   SLOT(slotRecordStart()));
  QObject::connect(ui->stopRecordButton, SIGNAL(clicked(bool)), this,
                   SLOT(slotRecordStop()));
  QObject::connect(m_pRecorder, SIGNAL(started()), this,
                   SLOT(slotRecorderStarted()));
  /*QObject::connect(m_pRecorder, SIGNAL(networkStatsUpdated(FeedCounter)),
                   this, SLOT(slotUpdateRecordStats(FeedCounter)),
     Qt::DirectConnection);*/
  QObject::connect(m_pRecorder, SIGNAL(error(QString)), this,
                   SLOT(slotRecordError(QString)));
  QObject::connect(m_pRecorder, SIGNAL(finished()), this,
                   SLOT(slotRecorderFinished()));
  QObject::connect(ui->selectInputFileButton, SIGNAL(clicked(bool)), this,
                   SLOT(slotSelectRecFile()));

  QObject::connect(ui->startPlayerButton, SIGNAL(clicked(bool)), this,
                   SLOT(slotPlayStart()));
  QObject::connect(m_pPlayer, SIGNAL(started()), this, SLOT(slotPlayStarted()));
  QObject::connect(m_pPlayer, SIGNAL(finished()), this,
                   SLOT(slotPlayFinished()));
  QObject::connect(m_pPlayer, SIGNAL(error(QString)), this,
                   SLOT(slotPlayError(QString)));
  QObject::connect(ui->stopPlayerButton, SIGNAL(clicked(bool)), this,
                   SLOT(slotPlayStop()));
  QObject::connect(ui->selectOutputFileButton, SIGNAL(clicked(bool)), this,
                   SLOT(slotSelectPlayerFile()));
  QObject::connect(ui->playRateSpinBox, SIGNAL(valueChanged(double)), m_pPlayer,
                   SLOT(setRate(double)), Qt::QueuedConnection);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::slotRecordStart() {
  if (ui->inputFilenameEdit->text().isEmpty()) {
    QMessageBox::critical(this, "Error", "No output filename", QMessageBox::Ok);
    return;
  }
  if (ui->inputAddressEdit->text().isEmpty() ||
      ui->inputPortEdit->text().isEmpty()) {
    QMessageBox::critical(this, "Error", "No host/port to connect",
                          QMessageBox::Ok);
    return;
  }
  AbstractInputFeed::Ptr pFeed(new TcpClientInputFeed(
      ui->inputAddressEdit->text(), ui->inputPortEdit->text().toInt()));
  m_pRecorder->setInput(pFeed);
  m_pRecorder->setFilename(ui->inputFilenameEdit->text());
  m_pRecorder->start();
}

void MainWindow::slotRecordStop() {
  m_pRecorder->requestInterruption();
  m_pRecorder->wait();
}

void MainWindow::slotUpdateRecordStats(const FeedCounter &stats) {
  qDebug() << "Recorder stats updated";
  ui->inBytesCountLabel->setText(formatBytesCount(stats.getBytes()));
  ui->inMsgsCountLabel->setText(QString("%1").arg(stats.getMessages()));
}

void MainWindow::slotRecordError(const QString &strDescr) {
  QMessageBox::critical(0, "Recorder error", strDescr, QMessageBox::Ok);
}

void MainWindow::slotRecorderStarted() {
  ui->inputFilenameEdit->setEnabled(false);
  ui->selectInputFileButton->setEnabled(false);
  ui->inputAddressEdit->setEnabled(false);
  ui->inputPortEdit->setEnabled(false);
  ui->startRecordButton->setEnabled(false);
  ui->stopRecordButton->setEnabled(true);
  setWindowTitle("[REC] MM2Capture");
}

void MainWindow::slotRecorderFinished() {
  ui->inputFilenameEdit->setEnabled(true);
  ui->selectInputFileButton->setEnabled(true);
  ui->inputAddressEdit->setEnabled(true);
  ui->inputPortEdit->setEnabled(true);
  ui->startRecordButton->setEnabled(true);
  ui->stopRecordButton->setEnabled(false);
  setWindowTitle("MM2Capture");
}

void MainWindow::slotSelectRecFile() {
  QFileDialog dialog(nullptr, "Open DB to write");
  // dialog.setFileMode(QFileDialog::AnyFile);
  QStringList filters;
  filters << "Any file (*)"
          << "SQLite3 DB (*.db)";
  dialog.setNameFilters(filters);

  int res = dialog.exec();

  if (QFileDialog::Accepted == res) {
    ui->inputFilenameEdit->setText(dialog.selectedFiles().at(0));
  }
}

void MainWindow::slotSelectPlayerFile() {
  QFileDialog dialog;
  QStringList filters;
  filters << "SQLite3 databases (*.db)"
          << "Any files(*)";
  dialog.setNameFilters(filters);
  int res = dialog.exec();
  QString fileName = dialog.selectedFiles().at(0);
  if (QFileDialog::Accepted == res)
    ui->outputFilenameEdit->setText(fileName);
}

void MainWindow::slotPlayStart() {
  if (ui->outputFilenameEdit->text().isEmpty()) {
    QMessageBox::critical(this, "Error", "No filename to play",
                          QMessageBox::Ok);
    return;
  }
  if (ui->outputAddressEdit->text().isEmpty() ||
      ui->outputPortEdit->text().isEmpty()) {
    QMessageBox::critical(this, "Error", "No host/port to output connect",
                          QMessageBox::Ok);
    return;
  }

  if (!m_pDbReader) {
    m_pDbReader = DBReader::Ptr(new DBReader());
  }
  m_pDbReader->setFile(ui->outputFilenameEdit->text());

  AbstractOutputFeed::Ptr pOutput(new TcpClientOutputFeed(
      ui->outputAddressEdit->text(), ui->outputPortEdit->text().toInt()));
  pOutput->setOutputType(ModesData::MessageType::Beast);
  m_pPlayer->setOutput(pOutput);

  try {
    m_pDbReader->open();

    OutputSessionDialog dialog;
    dialog.useReader(*m_pDbReader);

    if (dialog.exec() == QDialog::Accepted) {
      DBSessionStats sessionInfo;
      m_pDbReader->tryUseSession(dialog.selectedSessionId(), sessionInfo);
      m_pPlayer->setReader(m_pDbReader);
      m_pPlayer->start();
    }
  } catch (const std::runtime_error &exc) {
    QMessageBox::critical(this, "Error", QString(exc.what()), QMessageBox::Ok);
    return;
  }
}

void MainWindow::slotPlayStop() {
  m_pPlayer->requestInterruption();
  m_pPlayer->wait();
}

void MainWindow::slotPlayError(const QString &strDescr) {
  QMessageBox::critical(0, "Player error", strDescr, QMessageBox::Ok);
}

void MainWindow::slotPlayStarted() {
  ui->outputFilenameEdit->setEnabled(false);
  ui->outputAddressEdit->setEnabled(false);
  ui->outputPortEdit->setEnabled(false);
  ui->selectOutputFileButton->setEnabled(false);
  ui->startPlayerButton->setEnabled(false);
  ui->stopPlayerButton->setEnabled(true);
}

void MainWindow::slotPlayFinished() {
  ui->outputFilenameEdit->setEnabled(true);
  ui->outputAddressEdit->setEnabled(true);
  ui->outputPortEdit->setEnabled(true);
  ui->selectOutputFileButton->setEnabled(true);
  ui->startPlayerButton->setEnabled(true);
  ui->stopPlayerButton->setEnabled(false);
}

void MainWindow::slotRecordPlayed() {
  QMessageBox::information(0, "Finished", "Record played", QMessageBox::Ok);
}
