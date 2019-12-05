#include <iostream>

#include "display.hpp"

Progress::Progress(size_t max)
    : iteration{ 0 }, max{ (uint32_t)max }
{}

void Progress::update()
{
    iteration++;
    std::printf("\rIteration [%d / %d]", iteration, max);
    fflush(stdout);
    if (iteration == max)
        std::cout << std::endl;
}

Timer::Timer(std::string program_name)
    : program_name{program_name}
{}

void Timer::start()
{
    start_clock = std::chrono::high_resolution_clock::now();
    auto start_time = std::chrono::system_clock::now();
    auto start_timestamp = std::chrono::system_clock::to_time_t(start_time);

    std::cout << program_name << " started on " << std::ctime(&start_timestamp) << std::endl;
}

void Timer::stop()
{
    auto end_clock = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> duration = end_clock - start_clock;
    auto end_time = std::chrono::system_clock::now();
    auto end_timestamp = std::chrono::system_clock::to_time_t(end_time);

    std::cout << program_name << " finished on " << std::ctime(&end_timestamp)
              << "Elapsed time: " << duration.count() << "s." << std::endl;
}
