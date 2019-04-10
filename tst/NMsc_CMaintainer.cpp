//
// Created by ddl_blue on 18.10.18.
//

#include <cassert>
#include <unistd.h>
#include <atomic>

#include "../include/msc/CMaintainer.hpp"

constexpr int waitTime = 40 * 1000;

int main(int argc, const char *argv[]) {
    // todo why the hell is PortAudio starting? :D
    NMsc::CMaintainer &maintainer = NMsc::CMaintainer::GetInstance();
    assert(&maintainer != nullptr);

    std::atomic_int counter(0);
    auto counterTask = [&]() { ++counter; };

    int taskId = maintainer.RegisterTask(counterTask);
    usleep(waitTime);

    // Test if running
    assert(counter != 0);
    counter = 0;

    // Test Stop
    maintainer.Stop();
    usleep(waitTime);
    assert(counter == 0);
    counter = 0;

    // Test Run
    maintainer.Run();
    usleep(waitTime);
    assert(counter != 0);
    counter = 0;

    // Test Unregister
    maintainer.UnregisterTask(taskId);
    usleep(waitTime);
    assert(counter == 0);
    counter = 0;

    // Re-register
    int oldTaskId = taskId;
    taskId = maintainer.RegisterTask(counterTask);
    assert(oldTaskId != taskId);
    usleep(waitTime);
    assert(counter != 0);
    counter = 0;

    // Test ClearTaskList
    maintainer.ClearTaskList();
    counter = 0;
    usleep(waitTime);
    assert(counter == 0);

    return 0;
}