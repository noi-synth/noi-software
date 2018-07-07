//
// Created by ddl_blue on 7.7.18.
//

#ifndef NOI_SOFTWARE_CSNDHELPERS_HPP
#define NOI_SOFTWARE_CSNDHELPERS_HPP

#include <cmath>
#include "SndEnums.hpp"
#include "NSndConfig.hpp"

namespace NSnd {
    class CSndHelpers {
    public:
        static double GetSampleStep(ETones tone, int sampleLen);

    };
}

// TODO why does it not recompile when this file is changed?
/*##############################################################################*/

using namespace NSnd;

/*----------------------------------------------------------------------*/
double CSndHelpers::GetSampleStep(ETones tone, int sampleLen) {
    int relativeTone = tone - ETones::A4;

    // this is just a formula I made up. Might not be correct. Coding without internet connection... (works perfect with phone tuner though)
    // todo find the correct formula
    return sampleLen * exp2(relativeTone / 12.0) / SAMPLE_RATE * 440;

}


#endif //NOI_SOFTWARE_CSNDHELPERS_HPP
