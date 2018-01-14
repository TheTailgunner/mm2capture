/*! \file SQLite DB writing facility implementation
 */
#ifndef _DB_WRITER_H
#define _DB_WRITER_H

#include <QtSql>
#include <QString>
#include <QSharedPointer>
#include <QVector>
#include <stdexcept>

#include <modes/modes-data.h>

namespace MM2Capture {

/*!
 * \brief Breaks parsed data into the chunks, packs last ones and writes to DB.
 */
class DBWriter {
public:
  using Ptr = QSharedPointer<DBWriter>;
  DBWriter();
  /*!
   * \param [in] strFilename - file to write to
   */
  explicit DBWriter(const QString &strFilename);
  /// Sets file to write to
  void setFilename(const QString &strFn) { m_dbPath = strFn; }
  /*!
   * \brief Open connection.
   * \details Tries to open SQLite DB and put new session record;
   * If file is not exists, creates one.
   * \param sessionName
   * \throws std::runtime_exception in case of SQL error or if input
   * filename was not specified.
   */
  void open(const QString &sessionName);
  /// \returns true if connection is open.
  inline bool isOpened() const { return m_isOpened; }
  /*!
   * \brief Adds as many messages as possible.
   * \details Adds as many ass possible messages from beginning of msgs. Added
   * messages
   * erased from msgs.
   * \param [in] msgs - input messages list
   * \throws std::runtime_error if connection/session is not open.
   * \returns Number of messages added.
   * \warning Should be called in a loop until input messages list is not empty
   * or call returns not null value.
   */
  unsigned addMessages(QVector<ModesData> &msgs);
  /*!
   * \brief Flushes data to DB, updates session record and closes connection.
   */
  void close();
  ~DBWriter() { close(); }

private:
  static const QString CONNECTION_NAME;
  static const int MAX_MESSAGES_TO_FLUSH = 2000;

  QString m_dbPath;
  QString m_sessionName;
  bool m_isOpened;
  bool m_isSessionOpen;
  quint64 m_sessionId;
  QVector<ModesData> m_msgsToFlush; // current messages' chunk
  quint64 m_chunkNumber;

  QSqlError prepareTables(QSqlDatabase &);
  bool openSession(QSqlDatabase &, const QString &, QSqlError &);
  void flushChunk();
  bool closeSession(QSqlError &);
};
}
#endif
