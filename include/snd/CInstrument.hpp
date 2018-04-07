//
// Created by ddl_blue on 15.11.17.
//
#include <cstdint>

#ifndef NOI_SOFTWARE_CINSTRUMENT_HPP
#define NOI_SOFTWARE_CINSTRUMENT_HPP

namespace NSnd {
    class CInstrument {
    public:
        CInstrument();

        void GenerateBuffer(float *buffer, uint_fast32_t sampleCnt);


    };
}

#endif // NOI_SOFTWARE_CINSTRUMENT_HPP