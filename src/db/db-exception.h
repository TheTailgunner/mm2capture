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

#ifndef _DB_EXCEPTION_H_
#define _DB_EXCEPTION_H_

#include <QSqlError>

/*! \brief DB exception.
 * \details Contains native QtSql error object (QSqlError).
 */
struct DBException : public std::runtime_error {
public:
  //! Creates eception object with appoprite QSqlError
  DBException(const QSqlError &err)
      : std::runtime_error(err.text().toStdString()), m_errObj{err} {}
  //! Returns QSqlError description
  virtual const char *what() const throw() {
    return m_errObj.text().toStdString().c_str();
  }
  //! Returns native underlying QSqlError object
  inline const QSqlError &getSqlError() const { return m_errObj; }

private:
  QSqlError m_errObj;
};

#endif
