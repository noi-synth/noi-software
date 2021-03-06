//
// Created by ddl_blue on 6.4.18.
//

#ifndef NOI_SOFTWARE_NSNDCONFIG_HPP
#define NOI_SOFTWARE_NSNDCONFIG_HPP


#include <stdint.h>
#include <pthread.h>

#define SND_DATA_TYPE float

namespace NSnd {
    //TODO split compile-time config and runtime config

    /// Sample rate of the audio I/O stream
    const uint32_t SAMPLE_RATE = 44100;

    // const uint32_t TICK_LENGTH = 882; // 44100/50

    // Track slice length must be 2^n
    /// Track slice length must be 2^n.
    const uint32_t TRACK_SLICE_LEN_EXPONENT = 10;

    /// Length of single CTrackSlice audio buffer in frames
    constexpr uint32_t TRACK_SLICE_LEN = 1 << TRACK_SLICE_LEN_EXPONENT;
    /// Length of single CTrackSlice audio buffer in samples
    constexpr uint32_t TRACK_SLICE_BUFFER_LEN = TRACK_SLICE_LEN << 1; // two channels

    // const int SCHED_POLICY = SCHED_FIFO;

    /// Maximum number of voices in an instrument
    const int INSTRUMENT_VOICE_NUMBER = 12;

    /// Maximum length of audio processing request's buffer
    const int INTERNAL_BUFFERS_LEN = 1 << 12;
    //const int SCHED_PRIORITY_WORKER = ???

}


#endif //NOI_SOFTWARE_NSNDCONFIG_HPP
