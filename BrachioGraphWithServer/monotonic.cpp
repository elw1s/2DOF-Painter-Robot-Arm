#include <chrono>
#include <iostream>

int main() {
    // Get the current timepoint from steady clock
    auto currentTime = std::chrono::steady_clock::now();

    // Calculate the duration since the epoch
    auto duration = currentTime.time_since_epoch();

    // Convert the duration to seconds
    double seconds = std::chrono::duration<double>(duration).count();

    // Print the time in seconds since the epoch
    std::cout << "C++: Value of the monotonic clock (in seconds since epoch): " << seconds << std::endl;

    // Your Python execution command here
    system("python3 monotonic.py");

    return 0;
}
