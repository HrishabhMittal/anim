#include <chrono>

class Timer {
    using clock = std::chrono::steady_clock;
    clock::time_point start;
public:
    Timer(): start(clock::now()) {}
    double time() const {
        auto now = clock::now();
        std::chrono::duration<double> elapsed=now-start;
        return elapsed.count();
    }
};

