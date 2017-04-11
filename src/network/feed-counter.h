#ifndef _FEED_COUNTER_H
#define _FEED_COUNTER_H

namespace MM2Capture {

struct FeedCounter {
public:
    FeedCounter(): m_bytes{0L} {
    }

    void reset() {
        m_bytes = 0L;
    }

    void incBytes(unsigned long bytes) {
        m_bytes += bytes;
    }

    unsigned long getBytes() const {
        return m_bytes;
    }
private:
    unsigned long m_bytes;
};
}
#endif
