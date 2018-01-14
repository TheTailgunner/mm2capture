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

/*! \file
 * \brief Contains IO facility interface.
 */

#ifndef _ABSTRACT_IO_H
#define _ABSTRACT_IO_H

#include <QIntegerForSize>

class QByteArray;

namespace MM2Capture {

/*!
 * \brief Data IO implementation detail's interface.
 */
class AbstractIOImpl {
public:
  /*! Puts IO facility implementation into the working state. */
  virtual void run() = 0;
  /*! Terminates IO facility implementation. */
  virtual void terminate() = 0;
  /*!
   * \brief Reads all available bytes and rewrites passed array.
   * \param [out] out - byte array to store data (will be rewritten)
   * \returns Number of bytes read.
   */
  virtual qint64 read(QByteArray &out) = 0;
  /*!
   * \brief Writes passed byte array.
   * \param [in] in - data to write
   * \returns Number of bytes written.
   */
  virtual qint64 write(const QByteArray &in) = 0;
};
}

#endif
