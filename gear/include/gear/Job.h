//
// Created by felix on 30/07/2020.
//

#ifndef GEAR_JOB_H
#define GEAR_JOB_H

#include <functional>
#include <queue>
#include <vector>
#include <atomic>
#include <boost/context/fiber.hpp>
#include <optional>

namespace gear {

    struct Job {
        std::function<void(void)> fn;
    };

    struct WaitingJob {
        boost::context::fiber fiber;
        std::atomic<int>& waitOn;
    };

    struct Scheduler {
        boost::context::fiber fiber;
        std::queue<Job> jobs;
        std::vector<WaitingJob> waiting;

        void addJob(const std::function<void(void)>& fn);

        std::optional<WaitingJob> getWaitingJob();

        void resume();
    };

}

#endif //GEAR_JOB_H
