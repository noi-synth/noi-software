//
// Created by ddl_blue on 6.4.18.
//

#ifndef NOI_SOFTWARE_NSNDCONFIG_HPP
#define NOI_SOFTWARE_NSNDCONFIG_HPP


#include <stdint.h>
#include <pthread.h>

#define SND_DATA_TYPE float

namespace NSnd {
    const uint32_t SAMPLE_RATE = 44100;


    const int SCHED_POLICY = SCHED_FIFO;
    //const int SCHED_PRIORITY_WORHER = ???

}


#endif //NOI_SOFTWARE_NSNDCONFIG_HPP
