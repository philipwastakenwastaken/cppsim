#pragma once
#include "core/core.hpp"
#include "core/log.hpp"
#include "core/hardware.hpp"

#include "util/ring_buffer.hpp"
#include "util/timer.hpp"

#include <chrono>

namespace cppsim {

constexpr std::size_t FrameTimerSize = 128;
constexpr float TimeConversionFactor = 1000000000.0;

class FrameTimer : public RingBuffer<u64, FrameTimerSize>
{

    using Milli = std::chrono::milliseconds;
    using Micro = std::chrono::microseconds;
    using Nano = std::chrono::nanoseconds;

  public:
    FrameTimer();

    void update();
    void start();
    void stop();
    [[nodiscard]] u64 elapsed() const;

    [[nodiscard]] float fps() const;
    [[nodiscard]] float average() const;
    [[nodiscard]] float delta_time() const;
    [[nodiscard]] const std::array<float, FrameTimerSize>& lin() const { return linear_data; }
    //[[nodiscard]] inline std::array<u64, FrameTimerSize> linear() const { return m_buffer.linear(); }


  private:
    //RingBuffer<u64, FrameTimerSize> m_buffer;
    u64 m_sum;
    u64 m_delta_time;
    std::chrono::steady_clock::time_point begin, end;
    std::array<float, FrameTimerSize> linear_data;
};


} // namespace cppsim
