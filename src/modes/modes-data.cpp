#include <QDateTime>
#include <QDataStream>
#include <chrono>

#include "modes-data.h"

using namespace MM2Capture;

namespace MM2Capture {

QDataStream &operator<<(QDataStream &strm, const ModesData &msg) {
    strm.setVersion(QDataStream::Qt_4_0);
    strm << static_cast<qint32>(msg.m_type);
    strm << msg.m_timestamp;
    strm << msg.m_data;
    return strm;
}

}

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
