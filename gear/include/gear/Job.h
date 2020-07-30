//
// Created by felix on 30/07/2020.
//

#ifndef GEAR_JOB_H
#define GEAR_JOB_H

#include <functional>
#include <queue>
#include <vector>
#include <atomic>

namespace gear {

    struct Job {
        std::function<void(void)> fn;
    };

    struct WaitingJob {
        Job job;
        std::atomic<int> waitOn;
    };

    struct Scheduler {
        std::queue<Job> jobs;
        std::vector<WaitingJob> waiting;
        
    };

}

#endif //GEAR_JOB_H
