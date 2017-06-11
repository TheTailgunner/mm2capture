#include "db-chunk.h"
#include "../compress/packer.h"

#include <algorithm>

using namespace MM2Capture;

const int DBChunk::MAX_MESSAGES_COUNT = 2000;

DBChunk::DBChunk(): m_startTime{0}, m_validCompressed{false}
{ }

unsigned
DBChunk::addMessages(QVector<ModesData> &inVec)
{
    if (!inVec.size())
        return 0;
    /*! push_back in m_messages as many messages as possible
     * (m_messages.size() <= MAX_MESSAGES_COUNT) */
    int nAvail = MAX_MESSAGES_COUNT - m_messages.size();
    int nInMsgs = nAvail;
    if (inVec.size() <= nAvail)
        nInMsgs = inVec.size();
    std::copy_n(inVec.begin(), nInMsgs, std::back_inserter(m_messages));
    inVec.erase(inVec.begin(), inVec.begin() + nInMsgs);

    /*! if non-compressed queue is full, trying to compress */
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
            // compressed chunk is ready to write
            m_validCompressed = false;
        }
    }
    return nInMsgs;
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
