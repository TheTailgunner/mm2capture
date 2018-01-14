#include <QFile>
#include <QDataStream>
#include <QScopedArrayPointer>

#include "decoder-test.h"

using namespace MM2Capture;

void DecoderTest::testBeast() {
  static const int BEAST_MSG_COUNT = 11;
  static const QString fName = "test_data/beast.test";
  QFile beastFile(fName);
  beastFile.open(QIODevice::ReadOnly | QIODevice::Unbuffered);
  QVERIFY2(beastFile.isOpen() && beastFile.isReadable(), "Cannot read file");
  QDataStream inStream(&beastFile);
  quint64 bytesLeft = beastFile.bytesAvailable(), bytesRead = 0;
  QScopedArrayPointer<char> inBuf(new char[256]);
  unsigned nMsg = 0;
  QVector<ModesData> outMessages;
  while (bytesLeft > 0) {
    bytesRead = inStream.readRawData(inBuf.data(), 10);
    QByteArray data = QByteArray::fromRawData(inBuf.data(), bytesRead);
    bytesLeft -= bytesRead;
    nMsg += m_decoder.tryDecode(data, outMessages);
  }
  Q_ASSERT(nMsg == BEAST_MSG_COUNT);
}
