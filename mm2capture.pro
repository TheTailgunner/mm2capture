#-------------------------------------------------
#
# Project created by QtCreator 2017-04-09T00:26:45
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app
CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

tests {
message("Unit test build")
TARGET=mm2capture-tests
QT += testlib core
HEADERS += test/decoder-test.h \
	src/modes/modes-decoder.h \
	src/modes/modes-data.h
SOURCES += test/decoder-test.cpp \
	src/modes/modes-decoder.cpp \
        src/modes/modes-data.cpp \
        test/main.cpp
} else {
message("Normal build")
TARGET = mm2capture
SOURCES += src/main.cpp\
        src/mainwindow.cpp \
    src/network/base-input-feed.cpp \
    src/network/feed-counter.cpp \
    src/network/tcp-input-feed.cpp \
    src/modes/modes-decoder.cpp \
    src/modes/modes-data.cpp

HEADERS  += src/mainwindow.h \
    src/network/base-input-feed.h \
    src/network/feed-counter.h \
    src/network/tcp-input-feed.h \
    src/modes/modes-decoder.h \
    src/modes/modes-data.h

FORMS    += mainwindow.ui
}
