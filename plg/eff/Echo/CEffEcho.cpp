//
// Created by ddl_blue on 14.5.19.
//

#include "CEffEcho.hpp"

#define EchoLongLong(buffSrc, len, buffDst)   buffDst[(m_buffCursor + len) & (LONG_BUFF_LEN-1)]  += buffSrc[m_buffCursor] * m_absorbtion * (1-m_reverbPortion)

#define EchoLongShort(buffSrc, len, buffDst)  buffDst[(shortCursor  + len) & (SHORT_BUFF_LEN-1)] += buffSrc[m_buffCursor] * m_absorbtion * (m_reverbPortion / 2)

#define EchoShortShort(buffSrc, len, buffDst) buffDst[(shortCursor  + len) & (SHORT_BUFF_LEN-1)] += buffSrc[shortCursor]  * m_absorbtion / 3

#define EchoShortLong(buffSrc, len, buffDst)  buffDst[(m_buffCursor + len) & (LONG_BUFF_LEN-1)]  += buffSrc[shortCursor]  * m_absorbtion / 3


using namespace NPlg::NEcho;

/*----------------------------------------------------------------------*/
CEffEcho::CEffEcho() : m_buffCursor(0), m_size(10), m_delaySize(30000), m_absorbtion(0.9999999999999),
                       m_reverbPortion(1) {
    for (uint32_t i = 0; i < LONG_BUFF_LEN; ++i) {
        m_buffAl[i] = m_buffAr[i] = 0;
    }

    for (uint32_t i = 0; i < SHORT_BUFF_LEN; ++i) {
        m_buffBr[i] = m_buffBl[i] = m_buffCl[i] = m_buffCr[i] = 0;
    }
}

/*----------------------------------------------------------------------*/
void CEffEcho::ProcessBuffer(SND_DATA_TYPE *buffer, unsigned long buffLen) {

    // Inputs
    while (!m_paramChanges.Empty()) {
        CParamScroll change = m_paramChanges.Pop();

        switch (change.m_paramId) {
            case 0:
                if (change.m_directionUp && m_size < 11)
                    m_size += 0.25;
                else if (m_size > 1)
                    m_size -= 0.25;
                break;

            case 1:
                if (change.m_directionUp && m_absorbtion < 0.999999999)
                    m_absorbtion += 0.0001;
                else if (m_absorbtion > 0.9)
                    m_absorbtion -= 0.0001;
                break;

            case 2:
                if (change.m_directionUp && m_delaySize < LONG_BUFF_LEN * 0.9)
                    m_delaySize += (double) LONG_BUFF_LEN / 20;
                else if (m_delaySize > LONG_BUFF_LEN * 0.1)
                    m_delaySize -= (double) LONG_BUFF_LEN / 20;
                break;

            case 3:
                if (change.m_directionUp && m_reverbPortion < 1)
                    m_reverbPortion += 0.05;
                else if (m_reverbPortion > 0)
                    m_reverbPortion -= 0.05;
                break;

            default:
                break;
        }
    }

    // A
    uint32_t lnAlAr = (uint32_t) m_delaySize;
    uint32_t lnArAl = (uint32_t) m_delaySize;

    uint32_t lnAlBr = (uint32_t) (m_size * 7.1235732);
    uint32_t lnAlCr = (uint32_t) (m_size * 11.8635732);

    uint32_t lnArBl = (uint32_t) (m_size * 8.7412853);
    uint32_t lnArCl = (uint32_t) (m_size * 11.2212853);

    // B
    uint32_t lnBlCr = (uint32_t) (m_size * 677.234857);
    uint32_t lnBlAr = (uint32_t) (m_size * 13.374112);
    uint32_t lnBlCl = (uint32_t) (m_size * 137.10557697);

    uint32_t lnBrCl = (uint32_t) (m_size * 677.7364557);
    uint32_t lnBrAl = (uint32_t) (m_size * 13.97945612);
    uint32_t lnBrCr = (uint32_t) (m_size * 137.645645697);

    // C
    uint32_t lnClBr = (uint32_t) (m_size * 683.1832857);
    uint32_t lnClBl = (uint32_t) (m_size * 113.224112);
    uint32_t lnClAr = (uint32_t) (m_size * 17.44557697);

    uint32_t lnCrBl = (uint32_t) (m_size * 683.8327897);
    uint32_t lnCrBr = (uint32_t) (m_size * 113.7267812);
    uint32_t lnCrAl = (uint32_t) (m_size * 17.84312697);


    for (uint32_t i = 0; i < buffLen; ++i) {
        uint32_t shortCursor = m_buffCursor & (SHORT_BUFF_LEN - 1);

        buffer[i << 1] = m_buffAl[m_buffCursor] += buffer[i << 1];
        buffer[(i << 1) + 1] = m_buffAr[m_buffCursor] += buffer[(i << 1) + 1];

        // A
        EchoLongLong(m_buffAl, lnAlAr, m_buffAr);
        EchoLongLong(m_buffAr, lnArAl, m_buffAl);

        EchoLongShort(m_buffAl, lnAlBr, m_buffBr);
        EchoLongShort(m_buffAl, lnAlCr, m_buffCr);

        EchoLongShort(m_buffAr, lnArBl, m_buffBl);
        EchoLongShort(m_buffAr, lnArCl, m_buffCl);

        // B
        EchoShortShort(m_buffBl, lnBlCr, m_buffCr);
        EchoShortLong(m_buffBl, lnBlAr, m_buffAr);
        EchoShortShort(m_buffBl, lnBlCl, m_buffCl);

        EchoShortShort(m_buffBr, lnBrCl, m_buffCl);
        EchoShortLong(m_buffBr, lnBrAl, m_buffAl);
        EchoShortShort(m_buffBr, lnBrCr, m_buffCr);

        // C
        EchoShortShort(m_buffCl, lnClBr, m_buffBr);
        EchoShortShort(m_buffCl, lnClBl, m_buffBl);
        EchoShortLong(m_buffCl, lnClAr, m_buffAr);

        EchoShortShort(m_buffCr, lnCrBl, m_buffBl);
        EchoShortShort(m_buffCr, lnCrBr, m_buffBr);
        EchoShortLong(m_buffCr, lnCrAl, m_buffAl);


        m_buffAl[m_buffCursor] = m_buffAr[m_buffCursor] = m_buffBl[shortCursor] = m_buffBr[shortCursor] = m_buffCl[shortCursor] = m_buffCr[shortCursor] = 0;

        m_buffCursor = (m_buffCursor + 1) & (LONG_BUFF_LEN - 1);
    }


}

/*----------------------------------------------------------------------*/
void CEffEcho::FlushSound() {
    for (uint32_t i = 0; i < LONG_BUFF_LEN; ++i) {
        m_buffAl[i] = m_buffAr[i] = 0;
    }

    for (uint32_t i = 0; i < SHORT_BUFF_LEN; ++i) {
        m_buffBr[i] = m_buffBl[i] = m_buffCl[i] = m_buffCr[i] = 0;
    }
}