#ifndef _PACKER_H
#define _PACKER_H

#include "../modes/modes-data.h"
#include "zlib.h"

#include <QByteArray>
#include <QString>
#include <stdexcept>

namespace MM2Capture {

class PackerException: public std::exception {
public:
    explicit PackerException(int code): std::exception(),
        m_errMsg(PackerException::getZErrorStr(code)),
        m_errCode(code)
    {

    }
    virtual const char* what() const noexcept {
        return m_errMsg.toStdString().c_str();
    }
    inline int code() const {
        return m_errCode;
    }
private:
    static const char* getZErrorStr(int e) {
        switch(e) {
        case Z_ERRNO:
            return "zlib: input error";
        case Z_STREAM_ERROR:
            return "zlib: invalid compression level";
        case Z_DATA_ERROR:
            return "zlib: invalid data";
        case Z_MEM_ERROR:
            return "zlib: out of memory";
        case Z_VERSION_ERROR:
            return "zlib: version mismatch";
        }
        return "unknown error";
    }
    QString m_errMsg;
    int m_errCode;
};

class Packer {
public:
    Packer();
    Packer(const Packer &) =delete;
    Packer& operator=(const Packer &) =delete;
    static QByteArray compress(const QByteArray &);
    static QByteArray decompress(const QByteArray &);
};

}

#endif
