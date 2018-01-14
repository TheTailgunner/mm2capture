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

#ifndef _DB_SESSION_STATS_H
#define _DB_SESSION_STATS_H

namespace MM2Capture {

struct DBSessionStats {
  friend class DBReader;

public:
  DBSessionStats() : m_durationMsecs{0} {}
  unsigned long long duration() const { return m_durationMsecs; }

private:
  unsigned long long m_durationMsecs;
};
}

#endif
