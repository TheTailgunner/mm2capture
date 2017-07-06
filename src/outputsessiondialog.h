#ifndef OUTPUTSESSIONDIALOG_H
#define OUTPUTSESSIONDIALOG_H

#include <QDialog>
#include <QSqlQueryModel>
#include "db/db-reader.h"

namespace Ui {
class OutputSessionDialog;
}

class OutputSessionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OutputSessionDialog(QWidget *parent = 0);
    void useReader(const MM2Capture::DBReader &);
    quint64 selectedSessionId() const {
        return m_selectedId;
    }
    ~OutputSessionDialog();
private slots:
    void slotRowSelected(const QModelIndex &);
private:
    Ui::OutputSessionDialog *ui;
    QSqlQueryModel m_currentModel;
    quint64 m_selectedId;
};

#endif // OUTPUTSESSIONDIALOG_H
