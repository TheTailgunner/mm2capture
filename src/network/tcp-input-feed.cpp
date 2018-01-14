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

#include "tcp-input-feed.h"
#include <QByteArray>
#include <stdexcept>
#include <modes/modes-data.h>
#include <modes/modes-decoder.h>

using namespace MM2Capture;

TcpClientInputFeed::TcpClientInputFeed()
    : AbstractInputFeed(), TcpClientImpl(), m_isRunning{false},
      m_dataRead{false} {}

TcpClientInputFeed::TcpClientInputFeed(const QString &strHost, quint16 nPort)
    : AbstractInputFeed(), TcpClientImpl(), m_isRunning{false},
      m_dataRead{false} {
  TcpClientImpl::setHost(strHost);
  TcpClientImpl::setPort(nPort);
}

void TcpClientInputFeed::start() {
  if (this->TcpClientInputFeed::m_isRunning)
    return;
  TcpClientImpl::run();
  m_isRunning = true;
}

AbstractInputFeed &TcpClientInputFeed::operator>>(QVector<ModesData> &out) {
  if (!m_isRunning)
    m_dataRead = false;
  else {
    QByteArray data;
    try {
      qint64 nBytes = TcpClientImpl::read(data);
      m_stats.incBytes(nBytes);
      unsigned nMsg = 0;
      if (nBytes && (nMsg = m_pDecoder->tryDecode(data, out)) > 0) {
        m_dataRead = true;
        m_stats.incMessages(nMsg);
      }
    } catch (const std::runtime_error &exc) {
      (void)exc;
      m_dataRead = false;
    }
  }
  return *this;
}

void TcpClientInputFeed::stop() {
  TcpClientImpl::terminate();
  m_isRunning = false;
  m_dataRead = false;
}

TcpClientInputFeed::~TcpClientInputFeed() {
  if (m_isRunning) {
    TcpClientImpl::terminate();
    m_isRunning = false;
    m_dataRead = false;
  }
}
