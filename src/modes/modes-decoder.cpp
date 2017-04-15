#include "modes-decoder.h"

using namespace MM2Capture;

unsigned
DecoderBackendBeast::getFrameLength(char byte) {
    switch (byte) {
    case 0x31:
        /* here and now: 0x1A, frameType (1 byte), MLAT timestamp (6 bytes),
         * signal level (1 byte), various data length (depend on frameType)*/
        return 2 + 7 + 2;
    case 0x32:
        return 2 + 7 + 7;
    case 0x33:
        return 2 + 7 + 14;
    }
    return 0; // unknown
}

unsigned
DecoderBackendBeast::tryDecode(const QByteArray &data, ModesData &outMessage) {
    Q_ASSERT((data.size() - 1) < BUFFER_LENGTH_BYTES);
    unsigned messagesCount = 0;
    const char* pData = data.constData();
    int size = data.size();
    int nByte = 0;
    while (nByte < size){
        char byte = pData[nByte];
        switch (m_state) {
        case BeastDecoderState::FindEsc:
            if (0x1A == byte) {
                m_state = BeastDecoderState::FindType;
                m_nPayload = 0;
                m_buffer[m_nPayload] = byte;
                ++m_nPayload;
            }
            break;
        case BeastDecoderState::FindType:
            m_frameLength = getFrameLength(byte);
            if (!m_frameLength) { // wrong message type - probably not a message
                m_state = BeastDecoderState::FindEsc;
            }
            else {
                m_buffer[m_nPayload] = byte;
                m_state = BeastDecoderState::Data;
                ++m_nPayload;
            }
            break;
        case BeastDecoderState::Data:
            if (0x1A == byte) {
                m_state = BeastDecoderState::Quote;
            } else {
                m_state = BeastDecoderState::Data;
                m_buffer[m_nPayload] = byte;
                ++m_nPayload;
                if (m_nPayload == m_frameLength) {
                    outMessage.loadMessage(ModesData::MessageType::Beast,
                                           QByteArray(m_buffer.data(), m_frameLength));
                    ++messagesCount;
                    m_state = BeastDecoderState::FindEsc;
                }
            }
            break;
        case BeastDecoderState::Quote:
            if (byte != 0x1A) {
                m_state = BeastDecoderState::FindEsc;
                --nByte;
                continue;
            }
            else {
                m_state = BeastDecoderState::Data;
                m_buffer[m_nPayload] = byte;
                ++m_nPayload;
                if (m_nPayload == m_frameLength) {
                    outMessage.loadMessage(ModesData::MessageType::Beast,
                                           QByteArray(m_buffer.data(), m_frameLength));
                    ++messagesCount;
                    m_state = BeastDecoderState::FindEsc;
                }
            }
            break;
        default:
            m_state = BeastDecoderState::FindEsc;
        }
        ++nByte;
    }
    return messagesCount;
}

unsigned
ModesDecoder::tryDecode(const QByteArray &data, ModesData &outMsg)
{
    unsigned max = 0, nMsg = 0;
    if ((nMsg = m_pBeastDecoder->tryDecode(data, outMsg)) > max) {
        // BEAST data stream detected
        m_type = ModesData::MessageType::Beast;
        max = nMsg;
    }
    return max;
}
