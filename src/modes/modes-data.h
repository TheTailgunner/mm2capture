#ifndef _MODES_DATA_H
#define _MODES_DATA_H
#include <QByteArray>

namespace MM2Capture {

class ModesData {
public:
    enum class MessageType {
        None,
        Beast
    };
    ModesData(): m_type{MessageType::None}, m_timestamp{0} {
    }
    void loadMessage(MessageType, const QByteArray &);
    inline unsigned timestamp() const {
        return m_timestamp;
    }
    QByteArray serialize() const;
private:
    void loadMessageBeast(const QByteArray &);
    MessageType m_type;
    unsigned m_timestamp;
    QByteArray m_data; // complete AVR frame
};

}
#endif
