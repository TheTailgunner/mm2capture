#ifndef _FEED_COUNTER_H
#define _FEED_COUNTER_H

namespace MM2Capture {

struct FeedCounter {
public:
    FeedCounter() {
        reset();
    }

    void reset() {
        m_nBytes = 0L;
        m_nMessages = 0LL;
    }

    // [bytes]
    void incBytes(unsigned long bytes) {
        m_nBytes += bytes;
    }

    unsigned long getBytes() const {
        return m_nBytes;
    }

    // [messages]
    void incMessages(unsigned long m) {
        m_nMessages += m;
    }

    unsigned long getMessages() const {
        return m_nMessages;
    }
private:
    unsigned long long m_nBytes;
    unsigned long m_nMessages;
};
}
#endif
