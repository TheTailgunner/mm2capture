#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QElapsedTimer>

#include "db/db-reader.h"
#include "recorder.h"
#include "player.h"
#include "network/feed-counter.h"
#include "outputsessiondialog.h"

namespace Ui {
class MainWindow;
}

class QThread;
using namespace MM2Capture;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
signals:
    void started();
    void stopped();
public slots:
    void slotSelectFile();
private slots:
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
private:
    Ui::MainWindow *ui;
    MM2Capture::Recorder* m_pRecorder;
    MM2Capture::Player *m_pPlayer;
    DBReader::Ptr m_pDbReader;
};

#endif // MAINWINDOW_H
