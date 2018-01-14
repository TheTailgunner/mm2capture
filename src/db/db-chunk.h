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
    unsigned parseCompressed(const QByteArray &,
                             QVector<ModesData>);

    inline bool isValid() const {
        return m_validCompressed;
    }
    inline bool canFlush() const {
        return m_messages.size() > 0 || m_validCompressed;
    }
    inline quint64 startTime() const {
        return m_startTime;
    }
    inline const QByteArray& compressed() const {
        return m_compressed;
    }
    inline void clear() {
        m_messages.clear();
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
