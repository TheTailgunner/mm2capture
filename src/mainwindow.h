#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QElapsedTimer>

#include "recorder.h"
#include "network/feed-counter.h"

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
private:
    Ui::MainWindow *ui;
    MM2Capture::Recorder* m_pRecorder;
    QThread *m_pRecordThread;
};

#endif // MAINWINDOW_H
