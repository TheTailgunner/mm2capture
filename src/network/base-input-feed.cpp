#include "base-input-feed.h"

using namespace MM2Capture;

BaseInputSession::BaseInputSession(const BaseInputFeed::Ptr& pInput):
    m_pInput{pInput} {
}

void
BaseInputSession::start() {
}

void
BaseInputSession::stop() {
    if (m_pInput != nullptr)
        m_pInput->stop();
    m_isRunning = false;
}
