#pragma once
#include "core/core.hpp"
#include "core/log.hpp"
#include "core/hardware.hpp"

#include <algorithm>
#include <array>
#include <cstddef>


namespace cppsim {

template<typename T, std::size_t InputCapacity>
class RingBuffer
{
  public:
    RingBuffer() = default;
    [[nodiscard]] constexpr std::size_t capacity() const;

    void push_back(const T& item);
    [[nodiscard]] std::size_t size() const;
    [[nodiscard]] T next_ejected() const; // Returns 0 if size < capacity()
    // Copy to linear buffer.
    [[nodiscard]] std::array<T, std::bit_ceil(InputCapacity)> linear() const;

    // Most recently added element.
    [[nodiscard]] const T& first() const;

    T& operator[](std::size_t i);
    const T& operator[](std::size_t i) const;

  private:
    std::array<T, std::bit_ceil(InputCapacity)> m_data;
    std::size_t m_size = 0;
    std::size_t m_first = 0;
};

template<typename T, std::size_t InputCapacity>
constexpr std::size_t RingBuffer<T, InputCapacity>::capacity() const
{
    return std::bit_ceil<std::size_t>(InputCapacity);
}

template<typename T, std::size_t InputCapacity>
void RingBuffer<T, InputCapacity>::push_back(const T& item)
{
    m_data[m_first] = item;

    // Equivalent to (m_first + 1) % capacity()
    constexpr std::size_t one = 1;
    m_first = (m_first + one) & (capacity() - one);
    m_size = std::min(m_size + 1, capacity());
}

template<typename T, std::size_t InputCapacity>
T& RingBuffer<T, InputCapacity>::operator[](std::size_t i)
{
    CPPSIM_ASSERT(i < m_size, "Index of out bounds");
    return m_data[i];
}

template<typename T, std::size_t InputCapacity>
const T& RingBuffer<T, InputCapacity>::operator[](std::size_t i) const
{
    CPPSIM_ASSERT(i < m_size, "Index of out bounds");
    return m_data[i];
}


template<typename T, std::size_t InputCapacity>
std::size_t RingBuffer<T, InputCapacity>::size() const
{
    return m_size;
}


template<typename T, std::size_t InputCapacity>
T RingBuffer<T, InputCapacity>::next_ejected() const
{
    if (m_size < capacity())
        return 0;

    return m_data[m_first];
}

template<typename T, std::size_t InputCapacity>
const T& RingBuffer<T, InputCapacity>::first() const
{

    return m_data[m_first - 1];
}


template<typename T, std::size_t InputCapacity>
std::array<T, std::bit_ceil(InputCapacity)> RingBuffer<T, InputCapacity>::linear() const
{
    std::array<T, std::bit_ceil(InputCapacity)> out;

    std::rotate_copy(m_data.rbegin(), m_data.rbegin() + m_first, m_data.rend(), out.begin());


    return out;
}


template<typename T, std::size_t InputCapacity>
bool operator==(const RingBuffer<T, InputCapacity>& lhs, const RingBuffer<T, InputCapacity>& rhs)
{
    if (lhs.size() != rhs.size())
        return false;

    for (std::size_t i = 0; i < lhs.size(); i++)
        if (lhs[i] != rhs[i])
            return false;


    return true;
}


} // namespace cppsim
