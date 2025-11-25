#ifndef CLOCK_H
#define CLOCK_H

#include <iostream>
#include <chrono>
#include <thread> 

class Stopwatch {
private:
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = std::chrono::time_point<Clock>;

    TimePoint start_time;
    TimePoint end_time;
    bool is_running = false;

public:
    Stopwatch() = default;

    void start() {
        start_time = Clock::now();
        is_running = true;
        //std::cout << "Stopwatch started.\n";
    }

    void stop() {
        if (is_running) {
            end_time = Clock::now();
            is_running = false;
            //std::cout << "Stopwatch stopped.\n";
        } else {
            //std::cout << "Stopwatch is not running.\n";
        }
    }

    double get_elapsed_seconds() const {
        TimePoint current_time = is_running ? Clock::now() : end_time;

        auto duration = current_time - start_time;

        return std::chrono::duration<double>(duration).count();
    }
};

#endif
/*
int main() {
    Stopwatch timer;

    timer.start();

    std::cout << "Performing a simulated task...\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(2500));

    timer.stop();

    double elapsed = timer.get_elapsed_seconds();
    std::cout << "Task took: " << elapsed << " seconds.\n";

    timer.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout << "Lap time: " << timer.get_elapsed_seconds() << " seconds.\n";
    timer.stop();

    return 0;
}
*/
