//
// Created by ddl_blue on 8.4.18.
//



#include <thread>
#include <pthread.h>

bool setScheduling(std::thread &th, int policy, int priority) {
    sched_param sch_params;
    sch_params.sched_priority = priority;
    return pthread_setschedparam(th.native_handle(), policy, &sch_params) == 0;
}