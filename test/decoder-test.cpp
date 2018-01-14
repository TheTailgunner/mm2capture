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
