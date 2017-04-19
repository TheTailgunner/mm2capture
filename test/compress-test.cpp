#include "compress-test.h"

#include <QByteArray>
using namespace MM2Capture;

void
PackerTest::testPacker() {
    static char tmp[] = { 0xD, 0xE, 0xA, 0xD };
    QByteArray in(tmp, sizeof(tmp));
    QByteArray compressed = Packer::compress(in);
    QByteArray decompressed = Packer::decompress(compressed);
    Q_ASSERT(in == decompressed);
}
