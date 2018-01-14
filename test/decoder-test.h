#ifndef _DECODER_TEST_H
#define _DECODER_TEST_H
#include <QObject>
#include <QtTest/QtTest>

#include <modes/modes-decoder.h>
#include <modes/modes-data.h>

class DecoderTest : public QObject {
  Q_OBJECT
private:
  MM2Capture::ModesDecoder m_decoder;
private slots:
  void testBeast();
};

#endif
