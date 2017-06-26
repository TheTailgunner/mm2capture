#include "player.h"
#include <QSqlQuery>

using namespace MM2Capture;

Player::Player(QObject *prnt): QObject(prnt), m_isRunning{false}
{ }

void
Player::start() {
    if (m_isRunning)
        return;
    m_isRunning = true;
}

void
Player::stop() {
    if (!m_isRunning)
        return;
    m_isRunning = false;
}
