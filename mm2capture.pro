#-------------------------------------------------
#
# Project created by QtCreator 2017-04-09T00:26:45
#
#-------------------------------------------------

QT       += core gui network sql

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

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/zlib/release/ -lz
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/zlib/debug/ -lz
else:unix:!macx: LIBS += -L$$PWD/zlib/ -lz

INCLUDEPATH += $$PWD/zlib/include
DEPENDPATH += $$PWD/zlib/include
#QMAKE_LINK = clang++
#QMAKE_LFLAGS = -fsanitize=address
#QMAKE_CXXFLAGS += -fsanitize=address \
#   -fno-omit-frame-pointer \
#    -O1 -fno-optimize-sibling-calls

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/zlib/release/libz.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/zlib/debug/libz.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/zlib/release/z.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/zlib/debug/z.lib
else:unix:!macx: PRE_TARGETDEPS += $$PWD/zlib/libz.a

tests {
message("Unit test build")
TARGET=mm2capture-tests
QT += testlib core
HEADERS += test/decoder-test.h \
	src/modes/modes-decoder.h \
        src/modes/modes-data.h \
        src/compress/packer.h \
        test/compress-test.h
SOURCES += test/decoder-test.cpp \
	src/modes/modes-decoder.cpp \
        src/modes/modes-data.cpp \
        src/compress/packer.cpp \
        test/compress-test.cpp \
        test/main.cpp
# copying testdata
copydata.commands = $(COPY_DIR) $$PWD/test/test_data $$OUT_PWD
first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata
} else {
message("Normal build")
TARGET = mm2capture
SOURCES += src/main.cpp\
        src/mainwindow.cpp \
        src/outputsessiondialog.cpp \
    src/network/abstract-feed.cpp \
    src/network/feed-counter.cpp \
    src/network/tcp-input-feed.cpp \
    src/network/tcp-output-feed.cpp \
    src/network/io-impl/abstract-io.cpp \
    src/network/io-impl/tcp-client.cpp \
    src/modes/modes-decoder.cpp \
    src/modes/modes-data.cpp \
    src/compress/packer.cpp \
    src/db/db-exception.cpp \
    src/db/db-reader.cpp \
    src/db/db-writer.cpp \
    src/db/db-session-stats.cpp \
    src/network/feed-factory.cpp \
    src/recorder.cpp \
    src/player.cpp

HEADERS  += src/mainwindow.h \
    src/outputsessiondialog.h \
    src/network/abstract-feed.h \
    src/network/feed-counter.h \
    src/network/tcp-input-feed.h \
    src/network/tcp-output-feed.h \
    src/network/io-impl/abstract-io.h \
    src/network/io-impl/tcp-client.h \
    src/modes/modes-decoder.h \
    src/modes/modes-data.h \
    src/compress/packer.h \
    src/db/db-exception.h \
    src/db/db-reader.h \
    src/db/db-writer.h \
    src/db/db-session-stats.h \
    src/network/feed-factory.h \
    src/recorder.h \
    src/player.h \

FORMS    += mainwindow.ui \
    outputsessiondialog.ui
}



