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

#include "db-chunk.h"
#include "../compress/packer.h"

#include <algorithm>
#include <QDebug>
#include <QDataStream>

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
    std::copy_n(inVec.begin(), nInMsgs,
                std::back_inserter(m_messages));
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
        m_validCompressed = compress(chunk, m_compressed);
    }
    return nInMsgs;
}

unsigned
DBChunk::parseCompressed(const QByteArray &deflated,
                         QVector<ModesData> parsedMsgs) {
    QByteArray inflated;
    try {
       inflated = Packer::decompress(deflated);
    } catch (const PackerException &exc) {
        // TODO error logging
        (void) exc;
        return 0;
    }

    unsigned nAvail = inflated.size();
    QDataStream strm(&inflated,  QIODevice::ReadOnly);
    quint64 timestamp;
    quint8 frameLen;
    quint8 frame[255];

    while (nAvail) {
        strm >> timestamp;
        nAvail -= sizeof(quint64);
        strm >> frameLen;
        nAvail -= sizeof(quint8);
        nAvail -= strm.readRawData(
                    reinterpret_cast<char*>(frame),
                    frameLen
                    );
        parsedMsgs.push_back(ModesData::fromRaw(frame, timestamp, frameLen));
    }
    return parsedMsgs.size();
}

bool
DBChunk::flush(QByteArray &out)
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
    } catch (const PackerException& exc) {
        qDebug() << exc.what();
        return false;
    }
    return true;
}
