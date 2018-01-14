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

#ifndef _MODES_DATA_H
#define _MODES_DATA_H
#include <QDataStream>
#include <QByteArray>

namespace MM2Capture {

class ModesData {
public:
  enum class MessageType { None, Beast };
  ModesData() : m_type{MessageType::None}, m_timestamp{0} {}
  void loadMessage(MessageType, const QByteArray &);
  QByteArray getMessage(MessageType) const;
  inline quint64 timestamp() const { return m_timestamp; }

  friend QDataStream &operator<<(QDataStream &, const ModesData &);
  friend QDataStream &operator>>(QDataStream &, ModesData &);

private:
  const static std::size_t MLAT_LEN_BYTES = 6;

  MessageType m_type;
  quint64 m_timestamp;
  QByteArray m_data; //   complete AVR frame

  // BEAST thingies
  quint8 m_beastMlatTimestamp[MLAT_LEN_BYTES]{
      0x00}; //   MLAT section bytes for Beast
  quint8 m_beastSignalLevel{0x00};

  unsigned getBeastFrameLength(quint8);
  void loadMessageBeast(const QByteArray &);
  QByteArray getMessageBeast() const;
};
}
#endif
