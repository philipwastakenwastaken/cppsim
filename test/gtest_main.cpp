#include "gtest/gtest.h"
#include "core/log.hpp"


int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    cppsim::Log::init();

    return RUN_ALL_TESTS();
}
