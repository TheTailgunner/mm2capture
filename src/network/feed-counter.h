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
  FeedCounter() { reset(); }
  //! Resets all counters to initial values.
  void reset() {
    m_nBytes = 0L;
    m_nMessages = 0LL;
  }

  //! Increases processed bytes counter
  void incBytes(unsigned long bytes) { m_nBytes += bytes; }
  //! Returns processed bytes counter
  unsigned long getBytes() const { return m_nBytes; }

  //! Increases processed messages counter
  void incMessages(unsigned long m) { m_nMessages += m; }
  //! Returns processed messages counter
  unsigned long getMessages() const { return m_nMessages; }

private:
  unsigned long long m_nBytes;
  unsigned long m_nMessages;
};
}
#endif
