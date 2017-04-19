#ifndef _COMPRESS_TEST_H
#define _COMPRESS_TEST_H
#include <QObject>
#include <QtTest/QtTest>

#include "../src/compress/packer.h"

class PackerTest: public QObject {
    Q_OBJECT
private slots:
    void testPacker();
};

#endif
