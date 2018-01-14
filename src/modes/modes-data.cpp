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

#include <QDateTime>
#include <QDataStream>
#include <chrono>
#include <cstring>

#include "modes-data.h"

using namespace MM2Capture;

namespace MM2Capture {

QDataStream &operator<<(QDataStream &strm, const ModesData &msg) {
  strm << static_cast<qint32>(msg.m_type);
  strm << msg.m_timestamp;
  strm << msg.m_data;

  QByteArray mlat(reinterpret_cast<const char *>(msg.m_beastMlatTimestamp),
                  ModesData::MLAT_LEN_BYTES);
  strm << mlat;
  strm << msg.m_beastSignalLevel;
  return strm;
}

QDataStream &operator>>(QDataStream &strm, ModesData &msg) {
  qint32 intType;
  strm >> intType;
  msg.m_type = static_cast<ModesData::MessageType>(intType);

  strm >> msg.m_timestamp;
  strm >> msg.m_data;

  QByteArray mlat;
  strm >> mlat;
  strm >> msg.m_beastSignalLevel;
  return strm;
}
}

void ModesData::loadMessage(MessageType type, const QByteArray &data) {
  Q_ASSERT(type != MessageType::None);
  auto now = std::chrono::high_resolution_clock::now();
  m_timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
                    now.time_since_epoch()).count();

  switch (type) {
  case MessageType::Beast:
    loadMessageBeast(data);
    m_type = MessageType::Beast;
    break;
  default:
    break;
  }
}

QByteArray ModesData::getMessage(MessageType type) const {
  Q_ASSERT(type != MessageType::None);

  switch (type) {
  case MessageType::Beast:
    return getMessageBeast();
  default:
    break;
  }

  return QByteArray();
}

unsigned ModesData::getBeastFrameLength(quint8 typeByte) {
  switch (typeByte) {
  case 0x31:
    return 2;
  case 0x32:
    return 7;
  case 0x33:
    return 14;
  }
  return 0;
}

void ModesData::loadMessageBeast(const QByteArray &data) {
  unsigned frameLength = getBeastFrameLength(data[1]);

  std::memcpy(m_beastMlatTimestamp, data.data() + 2, MLAT_LEN_BYTES);
  m_beastSignalLevel = data[8];
  m_data.append(data.data() + 9, frameLength);
}

QByteArray ModesData::getMessageBeast() const {
  // result len = <esc> + type + mlat[6] + signal_lvl + framelen
  unsigned resLenBytes = 9 + m_data.length();
  QByteArray res(resLenBytes, 0x0);
  quint8 typeByte;

  switch (m_data.length()) {
  case 2:
    typeByte = 0x31;
    break;
  case 7:
    typeByte = 0x32;
    break;
  case 14:
    typeByte = 0x33;
    break;
  default:
    return QByteArray();
  }
  res[0] = 0x1A;
  res[1] = typeByte;
  std::memcpy(reinterpret_cast<void *>(res.data() + 2), m_beastMlatTimestamp,
              MLAT_LEN_BYTES);
  res[8] = m_beastSignalLevel;
  std::memcpy(reinterpret_cast<void *>(res.data() + 9), m_data.data(),
              m_data.length());
  return res;
}
