/* By Vladislav Nikolaev nvladislav9 at gmail dot com

This file is part of mm2capture.

mm2capture is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

mm2capture is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details. */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QElapsedTimer>

#include <db/db-reader.h>
#include "recorder.h"
#include "player.h"
#include <network/feed-counter.h>
#include <outputsessiondialog.h>

namespace Ui {
class MainWindow;
}

class QThread;
using namespace MM2Capture;

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();
signals:
  void started();
  void stopped();
private slots:
  void slotSelectRecFile();
  void slotSelectPlayerFile();

  void slotRecordStart();
  void slotRecordStop();
  void slotUpdateRecordStats(const FeedCounter &);
  void slotRecordError(const QString &);
  void slotRecorderStarted();
  void slotRecorderFinished();

  void slotPlayStart();
  void slotPlayStop();
  void slotPlayError(const QString &);
  void slotPlayStarted();
  void slotPlayFinished();
  void slotRecordPlayed();

private:
  Ui::MainWindow *ui;
  MM2Capture::Recorder *m_pRecorder;
  MM2Capture::Player *m_pPlayer;
  DBReader::Ptr m_pDbReader;
};

#endif // MAINWINDOW_H
