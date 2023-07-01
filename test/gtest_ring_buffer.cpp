#include "gtest/gtest.h"
#include "gtest/gtest-death-test.h"
#include <array>

#include "core/hardware.hpp"
#include "util/ring_buffer.hpp"

using namespace cppsim;

TEST(RingBuffer, capacity_scaled_to_power_of_two)
{
    RingBuffer<float, 8> x1;
    RingBuffer<float, 7> x2;
    RingBuffer<float, 118> x3;

    EXPECT_EQ(x1.capacity(), 8);
    EXPECT_EQ(x2.capacity(), 8);
    EXPECT_EQ(x3.capacity(), 128);
}

TEST(RingBuffer, push_back)
{
    RingBuffer<float, 8> x1;

    EXPECT_EQ(x1.size(), 0);

    x1.push_back(3.0f);
    EXPECT_EQ(x1.size(), 1);
    EXPECT_FLOAT_EQ(x1[0], 3);

    x1.push_back(4.0f);
    EXPECT_EQ(x1.size(), 2);
    EXPECT_FLOAT_EQ(x1[0], 3);
    EXPECT_FLOAT_EQ(x1[1], 4);
}

TEST(RingBuffer, push_back_wrap_around)
{
    RingBuffer<i32, 2> x1;

    x1.push_back(1);
    x1.push_back(2);

    EXPECT_EQ(x1.size(), 2);

    x1.push_back(3);

    EXPECT_EQ(x1.size(), 2);
    EXPECT_EQ(x1[0], 3);

    x1.push_back(4);

    EXPECT_EQ(x1.size(), 2);
    EXPECT_EQ(x1[1], 4);
}


TEST(RingBuffer, push_back_wrap_around2)
{
    RingBuffer<i32, 2> x1;

    x1.push_back(1);
    x1.push_back(2);

    EXPECT_EQ(x1.size(), 2);

    x1.push_back(3);
    x1.push_back(4);

    EXPECT_EQ(x1.size(), 2);
    EXPECT_EQ(x1[0], 3);

    x1.push_back(5);

    EXPECT_EQ(x1.size(), 2);
    EXPECT_EQ(x1[0], 5);
}


TEST(RingBuffer, push_back_out_of_bounds)
{
    RingBuffer<i32, 4> x1;

    EXPECT_DEATH(x1[0], "");
    EXPECT_DEATH(x1[5], "");
}

TEST(RingBuffer, next_ejected_under_capacity)
{
    RingBuffer<i32, 4> x1;

    x1.push_back(1);
    i32 ejected = x1.next_ejected();
    EXPECT_EQ(ejected, 0);

    x1.push_back(2);
    ejected = x1.next_ejected();
    EXPECT_EQ(ejected, 0);

    x1.push_back(3);
    ejected = x1.next_ejected();
    EXPECT_EQ(ejected, 0);
}

TEST(RingBuffer, next_ejected_at_capacity)
{
    RingBuffer<i32, 4> x1;

    x1.push_back(1);
    x1.push_back(2);
    x1.push_back(3);
    x1.push_back(4);

    EXPECT_EQ(x1.next_ejected(), 1);

    x1.push_back(10);
    EXPECT_EQ(x1.next_ejected(), 2);
}

TEST(RingBuffer, equal_operator_differing_sizes)
{
    RingBuffer<i32, 4> x1;
    RingBuffer<i32, 4> x2;


    x1.push_back(2);
    x1.push_back(1);

    x2.push_back(5);

    EXPECT_NE(x1, x2);
}


TEST(RingBuffer, equal_operator)
{
    RingBuffer<i32, 4> x1;
    RingBuffer<i32, 4> x2;


    x1.push_back(2);
    x1.push_back(1);
    x1.push_back(1);
    x1.push_back(1);

    x2.push_back(2);
    x2.push_back(1);
    x2.push_back(1);
    x2.push_back(1);

    EXPECT_EQ(x1, x2);
}