#include <QDateTime>

#include "modes-data.h"

using namespace MM2Capture;

void
ModesData::loadMessage(MessageType type, const QByteArray &data) {
    Q_ASSERT(type != MessageType::None);
    m_timestamp = QDateTime::currentDateTime().toTime_t();
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
