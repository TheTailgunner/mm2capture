#include <QtTest/QtTest>
#include "decoder-test.h"

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    DecoderTest decoderTest;
    return QTest::qExec(&decoderTest, argc, argv);
}
