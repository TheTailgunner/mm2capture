/*! \file Feed stats accumulator implementation.
 */
#ifndef _FEED_COUNTER_H
#define _FEED_COUNTER_H

namespace MM2Capture {

/*! \brief Feed stats accumulator implementation.
 * \details Used to store count of processed bytes, parsed messages etc.
 */
struct FeedCounter {
public:
    FeedCounter() {
        reset();
    }
    //! Resets all counters to initial values.
    void reset() {
        m_nBytes = 0L;
        m_nMessages = 0LL;
    }

    //! Increases processed bytes counter
    void incBytes(unsigned long bytes) {
        m_nBytes += bytes;
    }
    //! Returns processed bytes counter
    unsigned long getBytes() const {
        return m_nBytes;
    }

    //! Increases processed messages counter
    void incMessages(unsigned long m) {
        m_nMessages += m;
    }
    //! Returns processed messages counter
    unsigned long getMessages() const {
        return m_nMessages;
    }
private:
    unsigned long long m_nBytes;
    unsigned long m_nMessages;
};
}
#endif
