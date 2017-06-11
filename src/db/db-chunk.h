#ifndef DBCHUNK_H
#define DBCHUNK_H

#include <QVector>
#include <QByteArray>

#include "../modes/modes-data.h"

namespace MM2Capture {

class DBChunk
{
public:
    DBChunk();
    unsigned addMessages(QVector<ModesData> &);
    inline bool isValid() const {
        return m_validCompressed;
    }
    inline quint64 startTime() const {
        return m_startTime;
    }
    inline const QByteArray& compressed() const {
        return m_compressed;
    }
    inline bool canFlush() const {
        return  m_messages.empty() && m_validCompressed;
    }
    inline void clear() {
        m_compressed.clear();
        m_validCompressed = false;
        m_startTime = 0;
    }
    bool flush(QByteArray &);
private:
    static const int MAX_MESSAGES_COUNT;

    bool compress(const QByteArray &, QByteArray &);
    quint64 m_startTime;
    bool m_validCompressed;
    QVector<ModesData> m_messages;
    QByteArray m_compressed;
};

}
#endif // DBCHUNK_H
