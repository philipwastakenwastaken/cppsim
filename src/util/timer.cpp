#include "util/timer.hpp"
#include <chrono>

namespace cppsim {

void Timer::start() { begin = std::chrono::high_resolution_clock::now(); }

void Timer::stop() { end = std::chrono::high_resolution_clock::now(); }

} // namespace cppsim