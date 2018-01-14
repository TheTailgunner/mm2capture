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

#include "compress-test.h"

#include <QByteArray>
using namespace MM2Capture;

void PackerTest::testPacker() {
  static char tmp[] = {0xD, 0xE, 0xA, 0xD};
  QByteArray in(tmp, sizeof(tmp));
  QByteArray compressed = Packer::compress(in);
  QByteArray decompressed = Packer::decompress(compressed);
  Q_ASSERT(in == decompressed);
}
