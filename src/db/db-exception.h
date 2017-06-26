#ifndef _DB_EXCEPTION_H_
#define _DB_EXCEPTION_H_

/*! \brief DB exception.
 * \details Contains native QtSql error object (QSqlError).
 */
struct DBException: public std::runtime_error {
public:
    //! Creates eception object with appoprite QSqlError
    DBException(const QSqlError &err):
        std::runtime_error(err.text().toStdString()),
        m_errObj{err} {
    }
    //! Returns QSqlError description
    virtual const char *what() const throw() {
        return m_errObj.text().toStdString().c_str();
    }
    //! Returns native underlying QSqlError object
    inline const QSqlError &getSqlError() const {
        return m_errObj;
    }
private:
    QSqlError m_errObj;
};

#endif
