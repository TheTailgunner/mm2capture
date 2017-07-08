#include <QDateTime>
#include <QDataStream>
#include <chrono>

#include "modes-data.h"

using namespace MM2Capture;

void
ModesData::loadMessage(MessageType type, const QByteArray &data) {
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

void
ModesData::loadMessageBeast(const QByteArray &data) {
    unsigned frameLength = 0;
    uint8_t type = data[1];
    switch (type) {
    case 0x31:
        frameLength = 2;
        break;
    case 0x32:
        frameLength = 7;
        break;
    case 0x33:
        frameLength = 14;
        break;
    }
    m_data.append(data.data() + 9, frameLength);
}

QByteArray
ModesData::serialize() const
{
    QByteArray out;
    QDataStream strm(&out, QIODevice::WriteOnly);
    strm << m_timestamp;
    Q_ASSERT(m_data.length() <= 255);
    quint8 dataLen = m_data.length() & 0xFF;
    strm << dataLen;
    strm << m_data;
    return out;
}

ModesData
ModesData::fromRaw(const quint8 *pRaw, quint64 timestamp,
                   int len) {
    ModesData result;
    result.m_timestamp = timestamp;
    result.m_type = MessageType::None;
    result.m_data = std::move(QByteArray::fromRawData(
                           reinterpret_cast<const char*>(pRaw),
                           len
                           ));
    return result;
}
