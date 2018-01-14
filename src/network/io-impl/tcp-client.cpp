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

#include "tcp-client.h"
#include <QAbstractSocket>
#include <QByteArray>
#include <QDataStream>
#include <stdexcept>

using namespace MM2Capture;

TcpClientImpl::TcpClientImpl() : m_nPort{0}, m_isRunning{false} {}

TcpClientImpl::TcpClientImpl(const TcpClientImpl &obj) {
  m_strHost = obj.m_strHost;
  m_nPort = obj.m_nPort;
  m_isRunning = false;
}

void TcpClientImpl::run() {
  if (m_isRunning)
    return;
  m_socket.connectToHost(m_strHost, m_nPort);
  if (!m_socket.waitForConnected(CONNECT_TIMEOUT_MSEC)) {
    throw std::runtime_error(m_socket.errorString().toStdString());
  }
  m_isRunning = m_socket.state() == QAbstractSocket::ConnectedState;
}

void TcpClientImpl::terminate() {
  m_socket.close();
  m_isRunning = false;
}

qint64 TcpClientImpl::read(QByteArray &in) {
  if (!m_isRunning)
    throw std::runtime_error("TcpClientImpl::read(): did not started");
  if (!m_socket.waitForReadyRead(-1))
    throw std::runtime_error(m_socket.errorString().toStdString());
  qint64 nBytesToRead = m_socket.bytesAvailable();
  if (nBytesToRead)
    in = m_socket.readAll();
  return nBytesToRead;
}

qint64 TcpClientImpl::write(const QByteArray &in) {
  if (!m_isRunning)
    throw std::runtime_error("TcpClientImpl::read(): did not started");
  m_socket.write(in);
  if (!m_socket.waitForBytesWritten(-1)) {
    throw std::runtime_error(m_socket.errorString().toStdString());
  }
  return in.length();
}

TcpClientImpl::~TcpClientImpl() { terminate(); }
