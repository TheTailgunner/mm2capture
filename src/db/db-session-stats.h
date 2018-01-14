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
