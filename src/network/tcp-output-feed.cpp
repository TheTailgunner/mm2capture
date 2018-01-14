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

#include "tcp-output-feed.h"
#include <stdexcept>

using namespace MM2Capture;

TcpClientOutputFeed::TcpClientOutputFeed(const QString &strHost, quint16 nPort)
    : AbstractOutputFeed(), TcpClientImpl() {
  setHost(strHost);
  setPort(nPort);
}

void TcpClientOutputFeed::start() {
  if (this->AbstractOutputFeed::m_outputType == ModesData::MessageType::None)
    throw std::runtime_error("TcpClientOutputFeed::start() "
                             "output_type == None");
  TcpClientImpl::run();
}

TcpClientOutputFeed::~TcpClientOutputFeed() { stop(); }

void TcpClientOutputFeed::stop() { TcpClientImpl::terminate(); }

AbstractOutputFeed &TcpClientOutputFeed::operator<<(const ModesData &msg) {
  TcpClientImpl::write(msg.getMessage(m_outputType));
  return *this;
}
