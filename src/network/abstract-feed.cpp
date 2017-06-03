#include "abstract-feed.h"
#include "../modes/modes-decoder.h"

using namespace MM2Capture;

AbstractInputFeed::AbstractInputFeed():
    m_pDecoder{new ModesDecoder()} {
}
