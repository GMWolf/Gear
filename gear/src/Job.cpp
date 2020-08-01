//
// Created by felix on 30/07/2020.
//

#include <gear/Job.h>

void gear::Scheduler::addJob(const std::function<void(void)> &fn) {
    jobs.push({fn});
}

std::optional<gear::WaitingJob> gear::Scheduler::getWaitingJob() {
    for(auto p = waiting.begin(); p != waiting.end(); p++) {
        if (p->waitOn == 0) {
            WaitingJob job = std::move(*p);
            //waiting.erase(p);
            return job;
        }
    }

    return {};
}

void gear::Scheduler::resume() {
    auto wj = getWaitingJob();
    if (wj) {
        fiber = std::move(wj->fiber).resume();
    } else {
        fiber = boost::context::fiber(jobs.front().fn);
        jobs.pop();
    }
}
