#include "util/frame_timer.hpp"
#include <__chrono/duration.h>
#include <chrono>
#include <thread>

namespace cppsim {

FrameTimer::FrameTimer() : m_sum(0), m_delta_time(0) { end = std::chrono::high_resolution_clock::now(); }

void FrameTimer::update()
{
    start();

    u64 dt = elapsed();
    m_sum += dt - next_ejected();

    push_back(dt);
    m_delta_time = dt;

    for (i64 i = static_cast<i64>(size()) - 2; i >= 0; i--)
        linear_data[static_cast<u64>(i + 1)] = linear_data[static_cast<u64>(i)];
    linear_data[0] = dt;


    auto x = std::chrono::duration_cast<Nano>(begin - end);
    auto y = std::chrono::nanoseconds(16670000);
    auto z = std::chrono::duration_cast<std::chrono::nanoseconds>(y - x);

    CPPSIM_TRACE("{0} {1} {2}", dt / TimeConversionFactor, z.count() / 1000.0f, 0);
    if (z.count() > 0)
        std::this_thread::sleep_for(z);

    stop();
}

float FrameTimer::fps() const { return 1.0f / (average() / TimeConversionFactor); }

float FrameTimer::average() const { return m_sum / static_cast<float>(size()); }

float FrameTimer::delta_time() const { return m_delta_time / TimeConversionFactor; }


void FrameTimer::start() { begin = std::chrono::high_resolution_clock::now(); }

void FrameTimer::stop() { end = begin; }


u64 FrameTimer::elapsed() const
{
    return static_cast<unsigned long long>(std::chrono::duration_cast<Nano>(begin - end).count());
}


} // namespace cppsim