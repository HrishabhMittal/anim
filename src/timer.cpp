#include <chrono>
#include <iostream>
#include <ostream>
namespace Timer {
    namespace {
        using clock = std::chrono::steady_clock;
        clock::time_point start;
        double checkpoint=0;
        double frametime=-1.0;
        double lasttime=-1.0;
    }
    void startTimer() {
        start=clock::now();
    }
    double time() {
        auto now = clock::now();
        std::chrono::duration<double> elapsed=now-start;
        return elapsed.count();
    }
    double updateFrameTime() {
        if (frametime==-1) {
            frametime=time();
            lasttime=time();
            return frametime;
        }
        double curtime=time();
        frametime=curtime-lasttime;
        lasttime=curtime;
        return frametime;
    }
    double checkFrameTime() {
        return frametime;
    }
    void store() {
        checkpoint=time();
    }
    double check() {
        return checkpoint;
    }
};

