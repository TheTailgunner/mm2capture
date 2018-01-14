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

#ifndef _PACKER_H
#define _PACKER_H

#include <modes/modes-data.h>
#include "zlib.h"

#include <QByteArray>
#include <QString>
#include <stdexcept>

namespace MM2Capture {

class PackerException : public std::exception {
public:
  explicit PackerException(int code)
      : std::exception(), m_errMsg(PackerException::getZErrorStr(code)),
        m_errCode(code) {}
  virtual const char *what() const noexcept {
    return m_errMsg.toStdString().c_str();
  }
  inline int code() const { return m_errCode; }

private:
  static const char *getZErrorStr(int e) {
    switch (e) {
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
  Packer(const Packer &) = delete;
  Packer &operator=(const Packer &) = delete;
  static QByteArray compress(const QByteArray &);
  static QByteArray decompress(const QByteArray &);
};
}

#endif
