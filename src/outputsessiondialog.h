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

#ifndef OUTPUTSESSIONDIALOG_H
#define OUTPUTSESSIONDIALOG_H

#include <QDialog>
#include <QSqlQueryModel>
#include <db/db-reader.h>

namespace Ui {
class OutputSessionDialog;
}

class OutputSessionDialog : public QDialog {
  Q_OBJECT

public:
  explicit OutputSessionDialog(QWidget *parent = 0);
  void useReader(const MM2Capture::DBReader &);
  quint64 selectedSessionId() const {
    // FIXME SQL (at least SQLite) primary keys started at 1
    return m_selectedId + 1;
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
