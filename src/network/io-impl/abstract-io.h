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
    virtual void start() = 0;
    /*! Terminates IO facility implementation. */
    virtual void stop() = 0;
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
