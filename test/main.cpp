#include <QtTest/QtTest>
#include "decoder-test.h"
#include "compress-test.h"

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    DecoderTest decoderTest;
    PackerTest packerTest;
    return QTest::qExec(&decoderTest, argc, argv) | QTest::qExec(&packerTest, argc, argv);
}
