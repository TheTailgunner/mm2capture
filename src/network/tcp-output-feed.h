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

#ifndef _TCP_OUTPUT_FEED_H
#define _TCP_OUTPUT_FEED_H

#include "abstract-feed.h"
#include "io-impl/tcp-client.h"

namespace MM2Capture {

class ModesData;

class TcpClientOutputFeed : public AbstractOutputFeed, protected TcpClientImpl {
public:
  TcpClientOutputFeed() = default;
  TcpClientOutputFeed(const QString &, quint16);
  ~TcpClientOutputFeed();

  void start() override;
  void stop() override;

  void setHost(const QString &strHost) { TcpClientImpl::setHost(strHost); }

  void setPort(quint16 nPort) { TcpClientImpl::setPort(nPort); }

  AbstractOutputFeed &operator<<(const ModesData &) override;
};
}

#endif
