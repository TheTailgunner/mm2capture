#include "db-chunk.h"
#include "../compress/packer.h"

#include <algorithm>

using namespace MM2Capture;

const int DBChunk::MAX_MESSAGES_COUNT = 200;

DBChunk::DBChunk(): m_startTime{0}, m_validCompressed{false}
{ }

unsigned
DBChunk::addMessages(QVector<ModesData> &inVec)
{
    if (!inVec.size())
        return 0;
    int count = MAX_MESSAGES_COUNT - m_messages.size();
    /* поместить в messages столько сообщенийи з входной очереди, сколько можно *
     * (m_messages.size() <= MAX_MESSAGES_COUNT */
    std::copy_n(inVec.begin(), count, std::back_inserter(m_messages));
    /* если m_messages.size() == MAX_MESSAGES_COUNT, сжать */
    if (m_messages.size() == MAX_MESSAGES_COUNT) {
        // start time of the chunk == timestamp of the first messages in the chunk
        if (m_startTime == 0) {
            m_startTime = m_messages[0].timestamp();
        }
        QByteArray chunk;
        while (!m_messages.empty()) {
            chunk.append(m_messages[0].serialize());
            m_messages.pop_front();
        }
        try {
            m_validCompressed = compress(chunk, m_compressed);
        } catch (const PackerException&) {
            m_validCompressed = false;
        }
    }
    return count;
}

bool DBChunk::flush(QByteArray &out)
{
    QByteArray chunk;
    unsigned size = m_messages.size();
    for (unsigned i = 0; i < size; ++i) {
        chunk.append(m_messages[i].serialize());
    }
    clear();
    return compress(chunk, out);
}

bool
DBChunk::compress(const QByteArray &inData, QByteArray &outData)
{
    try {
        outData = Packer::compress(inData);
    } catch (const PackerException&) {
        return false;
    }
    return true;
}
