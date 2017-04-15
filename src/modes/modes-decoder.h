#ifndef _MODES_DECODER_H
#define _MODES_DECODER_H
#include <QScopedPointer>
#include <QScopedArrayPointer>

#include "modes-data.h"

namespace MM2Capture {

namespace {

class DecoderBackendBeast {
public:
    DecoderBackendBeast(): m_nPayload{0}, m_state{BeastDecoderState::FindEsc},
        m_buffer{new char[BUFFER_LENGTH_BYTES]} {
    }
    unsigned tryDecode(const QByteArray &, ModesData &);
private:
    enum class BeastDecoderState {
        FindEsc,
        FindType,
        Data,
        Quote
    };

    static const int BUFFER_LENGTH_BYTES = 128;

    unsigned getFrameLength(char);
    unsigned m_nPayload; // current m_buffer length
    unsigned m_frameLength; // expected frame length
    BeastDecoderState m_state;
    QScopedArrayPointer<char> m_buffer; // current frame data
};

}

class ModesDecoder {
public:
    ModesDecoder(): m_type{ModesData::MessageType::None},
        m_pBeastDecoder{new DecoderBackendBeast()} {
    }
    unsigned tryDecode(const QByteArray &, ModesData &outMsg);
private:
    ModesData::MessageType m_type;
    QScopedPointer<DecoderBackendBeast> m_pBeastDecoder;
};

}
#endif
