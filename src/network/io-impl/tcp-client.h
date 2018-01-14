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

/*! \file
 * \brief Contains TCP client IO implementation.
 */
#ifndef _TCP_CLIENT_H
#define _TCP_CLIENT_H

#include "abstract-io.h"
#include <QString>
#include <QTcpSocket>

class QByteArray;

namespace MM2Capture {

/*!
 * \brief Blocking TCP client implementation.
 * \todo Implement TCP Keepalive.
 */

class TcpClientImpl : public AbstractIOImpl {
public:
  TcpClientImpl();
  /*! \brief Creates TcpClientImpl from existing one.
   * \warning Created instance will not be in running state (needs to be
   * started).
   */
  TcpClientImpl(const TcpClientImpl &obj);
  TcpClientImpl &operator=(const TcpClientImpl &) = delete;
  void run() override;
  void terminate() override;
  /*! \exception Throws std::runtime_exception() if error occured.
   */
  qint64 read(QByteArray &in) override;
  /*! \exception Throws std::runtime_exception() if error occured.
   */
  qint64 write(const QByteArray &in) override;

  /*!
   * \brief Sets the hostname/IP to connect.
   * \param [in] strHost - hostname/IP
   */
  void setHost(const QString &strHost) { m_strHost = strHost; }
  /// Returns hostname.
  const QString &host() const { return m_strHost; }
  /*!
   * \brief Sets destination port.
   * \param [in] nPort - destination port
   */
  void setPort(quint16 nPort) { m_nPort = nPort; }
  /// Returns destination port.
  quint16 port() const { return m_nPort; }
  /// Is IO facility running?
  bool isRunning() const { return m_isRunning; }
  ~TcpClientImpl();

private:
  const static int CONNECT_TIMEOUT_MSEC = 3000;
  QString m_strHost;
  quint16 m_nPort;
  bool m_isRunning;
  QTcpSocket m_socket;
};
}

#endif
