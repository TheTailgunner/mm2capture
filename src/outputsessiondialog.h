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
    void useReader(const DBReader &);
    quint64 selectedSessionId() const;
    ~OutputSessionDialog();
private slots:
    void slotAccepted();
private:
    Ui::OutputSessionDialog *ui;
    QSqlQueryModel m_currentModel;
    quint64 m_selectedId;
};

#endif // OUTPUTSESSIONDIALOG_H
