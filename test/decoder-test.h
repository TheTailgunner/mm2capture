#ifndef _DECODER_TEST_H
#define _DECODER_TEST_H
#include <QObject>
#include <QtTest/QtTest>

#include "../src/modes/modes-decoder.h"
#include "../src/modes/modes-data.h"

class DecoderTest: public QObject {
    Q_OBJECT
private:
    MM2Capture::ModesDecoder m_decoder;
private slots:
    void testBeast();
};

#endif
