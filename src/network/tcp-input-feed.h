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

#ifndef _TCP_INPUT_FEED_H
#define _TCP_INPUT_FEED_H

#include <QVector>

#include "abstract-feed.h"
#include "io-impl/tcp-client.h"
#include <modes/modes-data.h>

namespace MM2Capture {

class TcpClientInputFeed : public AbstractInputFeed, protected TcpClientImpl {
public:
  TcpClientInputFeed();
  TcpClientInputFeed(const QString &strHost, quint16 nPort);
  ~TcpClientInputFeed();

  void start() override;
  void stop() override;

  operator bool() const { return m_dataRead; }

  void setHost(const QString &strHost) { TcpClientImpl::setHost(strHost); }

  void setPort(quint16 nPort) { TcpClientImpl::setPort(nPort); }

  AbstractInputFeed &operator>>(QVector<ModesData> &out) override;

private:
  bool m_isRunning;
  bool m_dataRead;
};
}
#endif
