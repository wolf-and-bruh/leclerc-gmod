#include "../includes.hpp"

scheduler::scheduler() : stopFlag(false) {}

scheduler::~scheduler() {
    stop();
}

void scheduler::init() {
    worker = std::thread([this]() { run(); });
    spdlog::default_logger()->info("[scheduler] Initialized successfully!");
}

void scheduler::stop() {
    {
        std::lock_guard<std::mutex> lock(mtx);
        stopFlag = true;
    }
    cv.notify_all();
    if (worker.joinable())
        worker.join();
    spdlog::default_logger()->info("[scheduler] Stopped!");
}

void scheduler::queue(std::function<void()> func) {
    queueDelayed(std::move(func), std::chrono::milliseconds(0));
    spdlog::default_logger()->info("[scheduler] Queued immediate function!");
}

void scheduler::queueDelayed(std::function<void()> func, std::chrono::milliseconds delay) {
    {
        std::lock_guard<std::mutex> lock(mtx);
        tasks.push(ScheduledTask{
            std::move(func),
            std::chrono::steady_clock::now() + delay,
            std::chrono::milliseconds(0)
            });
    }
    cv.notify_one();
    spdlog::default_logger()->info("[scheduler] Queued delayed function ({} ms)!", delay.count());
}

void scheduler::queueRepeating(std::function<void()> func, std::chrono::milliseconds interval) {
    {
        std::lock_guard<std::mutex> lock(mtx);
        tasks.push(ScheduledTask{
            std::move(func),
            std::chrono::steady_clock::now() + interval,
            interval
            });
    }
    cv.notify_one();
    spdlog::default_logger()->info("[scheduler] Queued repeating function ({} ms)!", interval.count());
}

void scheduler::run() {
    while (true) {
        std::vector<ScheduledTask> readyTasks;

        {
            std::unique_lock<std::mutex> lock(mtx);

            if (tasks.empty()) {
                cv.wait(lock, [this]() { return stopFlag || !tasks.empty(); });
            }
            else {
                auto nextTime = tasks.top().executeAt;
                cv.wait_until(lock, nextTime, [this]() {
                    return stopFlag || (!tasks.empty() && tasks.top().executeAt <= std::chrono::steady_clock::now());
                    });
            }

            if (stopFlag && tasks.empty())
                break;

            auto now = std::chrono::steady_clock::now();

            // collect ALL tasks that are due now
            while (!tasks.empty() && tasks.top().executeAt <= now) {
                auto task = tasks.top();
                tasks.pop();
                readyTasks.push_back(std::move(task));
            }
        }

        // execute tasks outside the lock
        for (auto& task : readyTasks) {
            //spdlog::default_logger()->info("[scheduler] Executing task!");
            if (task.func) task.func();

            // reschedule repeating
            if (task.interval.count() > 0) {
                task.executeAt += task.interval; // fixed, keeps steady interval
                {
                    std::lock_guard<std::mutex> lock(mtx);
                    tasks.push(std::move(task));
                }
                cv.notify_one();
            }
        }
    }
}



// define global instance
std::unique_ptr<scheduler> scheduler_system = std::make_unique<scheduler>();