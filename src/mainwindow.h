#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "recorder.h"
#include "network/base-input-feed.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void slotStart();
    void slotStop();
private:
    Ui::MainWindow *ui;
    MM2Capture::Recorder* m_pRecorder;
    MM2Capture::BaseInputFeed::Ptr m_feed;
};

#endif // MAINWINDOW_H
