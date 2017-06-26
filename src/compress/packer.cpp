#include "packer.h"

#include <QScopedArrayPointer>

using namespace MM2Capture;

QByteArray
Packer::compress(const QByteArray &data)
{
    Q_ASSERT(data.size() > 0);
    static const unsigned CHUNK_LEN_BYTES = 256;
    static Bytef out[CHUNK_LEN_BYTES]{0};
    char* pData = const_cast<char*>(data.data());
    int dataLen = data.size();
    z_stream zStream;
    zStream.zalloc = Z_NULL;
    zStream.zfree = Z_NULL;
    zStream.opaque = Z_NULL;

    int ret = deflateInit(&zStream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK)
        throw PackerException(ret);
    zStream.avail_in = dataLen;
    zStream.next_in = reinterpret_cast<Bytef*>(pData);
    ret = Z_OK;
    QByteArray compressed;
    do {
        zStream.avail_out = CHUNK_LEN_BYTES;
        zStream.next_out = out;
        ret = deflate(&zStream, Z_FINISH);
        if (ret < 0)
            throw PackerException(ret);
        compressed += QByteArray(reinterpret_cast<char*>(out),
                                 CHUNK_LEN_BYTES - zStream.avail_out);
    } while (zStream.avail_out == 0);
    Q_ASSERT(static_cast<unsigned long>(compressed.size()) == zStream.total_out);
    deflateEnd(&zStream);
    return compressed;
}

QByteArray
Packer::decompress(const QByteArray &data)
{
    Q_ASSERT(data.size() > 0);
    static const unsigned CHUNK_LEN_BYTES = 256;
    static Bytef chunk[CHUNK_LEN_BYTES]{0};
    char* pCompressed = const_cast<char*>(data.data());
    int compressedLen = data.size();
    z_stream zStream;
    QByteArray outData;
    zStream.zalloc = Z_NULL;
    zStream.zfree = Z_NULL;
    zStream.opaque = Z_NULL;
    int ret = inflateInit(&zStream);
    if (ret != Z_OK)
        throw PackerException(ret);
    zStream.avail_in = compressedLen;
    zStream.next_in = reinterpret_cast<Bytef*>(pCompressed);
    do {
        zStream.avail_out = CHUNK_LEN_BYTES;
        zStream.next_out = chunk;
        ret = inflate(&zStream, Z_FINISH);
        if (ret < 0)
            throw PackerException(ret);
        outData.append(reinterpret_cast<char*>(chunk),
                       CHUNK_LEN_BYTES - zStream.avail_out);
    } while (zStream.avail_out == 0);
    return outData;
}
