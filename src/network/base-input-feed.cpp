#include "base-input-feed.h"

using namespace MM2Capture;

BaseInputSession::BaseInputSession(BaseInputFeed* pInput):
    m_pInput{pInput} {
}

void
BaseInputSession::start() {
    m_isRunning = true;
}

void
BaseInputSession::stop() {
    if (m_pInput != nullptr)
        m_pInput->stop();
    m_isRunning = false;
}
