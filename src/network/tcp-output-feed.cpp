#include "tcp-output-feed.h"
#include <stdexcept>

using namespace MM2Capture;

TcpClientOutputFeed::TcpClientOutputFeed(const QString &strHost, quint16 nPort)
    : AbstractOutputFeed(), TcpClientImpl() {
  setHost(strHost);
  setPort(nPort);
}

void TcpClientOutputFeed::start() {
  if (this->AbstractOutputFeed::m_outputType == ModesData::MessageType::None)
    throw std::runtime_error("TcpClientOutputFeed::start() "
                             "output_type == None");
  TcpClientImpl::run();
}

TcpClientOutputFeed::~TcpClientOutputFeed() { stop(); }

void TcpClientOutputFeed::stop() { TcpClientImpl::terminate(); }

AbstractOutputFeed &TcpClientOutputFeed::operator<<(const ModesData &msg) {
  TcpClientImpl::write(msg.getMessage(m_outputType));
  return *this;
}
