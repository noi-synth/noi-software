//
// Created by ddl_blue on 6.4.18.
//

#include <stdio.h>

#include "../include/snd/CAudioDevice.hpp"
#include "../include/msc/CLogger.hpp"


uint8_t tn;

int callback(const SND_DATA_TYPE *in, SND_DATA_TYPE *out,
             unsigned long num) {
    for (uint32_t i = 0; i < num; ++i) {
        out[i * 2] = out[i * 2 + 1] = ((unsigned char) ++tn) / 256.0f;
    }


}

int main(int argc, const char *argv[]) {
    NSnd::CAudioDevice dev(callback, NSnd::CAudioDeviceConfig());


    if (dev.Open()) {
        dev.StartStream();
        NMsc::CLogger::Log("Stream started.");

        while (1) {

        }

    } else {
        NMsc::CLogger::Log("FAIL");
    }


    return 0;

}