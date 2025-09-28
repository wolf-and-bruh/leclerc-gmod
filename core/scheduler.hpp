#include "../includes.hpp"

struct ScheduledTask {
    std::function<void()> func;
    std::chrono::steady_clock::time_point executeAt;
    std::chrono::milliseconds interval; // 0 = one-shot, >0 = repeating

    bool operator>(const ScheduledTask& other) const {
        return executeAt > other.executeAt;
    }
};

class scheduler {
public:
    scheduler();
    ~scheduler();

    void init();
    void stop();

    void queue(std::function<void()> func);
    void queueDelayed(std::function<void()> func, std::chrono::milliseconds delay);
    void queueRepeating(std::function<void()> func, std::chrono::milliseconds interval);

private:
    void run();

    std::thread worker;
    std::mutex mtx;
    std::condition_variable cv;
    std::priority_queue<ScheduledTask, std::vector<ScheduledTask>, std::greater<>> tasks;
    std::atomic<bool> stopFlag;
};

// global instance
extern std::unique_ptr<scheduler> scheduler_system;