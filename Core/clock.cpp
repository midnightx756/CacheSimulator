#include <iostream>
#include <chrono>
#include <thread> // Only used for the sleep example in main()

class Stopwatch {
private:
    // Alias for the clock type and time point type for cleaner code
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = std::chrono::time_point<Clock>;

    TimePoint start_time;
    TimePoint end_time;
    bool is_running = false;

public:
    Stopwatch() = default;

    // Starts the stopwatch
    void start() {
        start_time = Clock::now();
        is_running = true;
        std::cout << "Stopwatch started.\n";
    }

    // Stops the stopwatch
    void stop() {
        if (is_running) {
            end_time = Clock::now();
            is_running = false;
            std::cout << "Stopwatch stopped.\n";
        } else {
            std::cout << "Stopwatch is not running.\n";
        }
    }

    // Gets the elapsed time since 'start' was called.
    // If the watch is running, it returns time up to this moment.
    // If stopped, it returns the time between start() and stop().
    double get_elapsed_seconds() const {
        TimePoint current_time = is_running ? Clock::now() : end_time;

        // Calculate the duration between the start and current/end time
        auto duration = current_time - start_time;

        // Convert the duration to seconds as a double-precision floating-point number
        // std::chrono::duration<double> specifies the target unit and type
        return std::chrono::duration<double>(duration).count();
    }
};

int main() {
    Stopwatch timer;

    // --- Start the timer ---
    timer.start();

    // Simulate work by making the program sleep for 2.5 seconds
    std::cout << "Performing a simulated task...\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(2500));

    // --- Stop the timer ---
    timer.stop();

    // --- Get and display the result ---
    double elapsed = timer.get_elapsed_seconds();
    std::cout << "Task took: " << elapsed << " seconds.\n";

    // You can also check elapsed time while it's still running (like a lap timer)
    timer.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout << "Lap time: " << timer.get_elapsed_seconds() << " seconds.\n";
    timer.stop();

    return 0;
}
