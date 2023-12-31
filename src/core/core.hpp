#pragma once
#include <cstdlib>
#include <string>

constexpr const char* WindowTitle = "cppsim";
constexpr int WindowWidth = 1280;
constexpr int WindowHeight = 720;


constexpr int GLContextVersionMajor = 4;
constexpr int GLContextVersionMinor = 1;

// Controls parameters related to building in different modes

#ifdef CPPSIM_DEBUG
    #define CPPSIM_ENABLE_ASSERT
    #define CPPSIM_ENABLE_LOGGING
#elif CPPSIM_RELEASE
    #define CPPSIM_ENABLE_ASSERT
    #define CPPSIM_ENABLE_LOGGING
#endif

#define CPPSIM_STATIC_ASSERT(x, ...) {static_assert(x, __VA_ARGS__);}

#ifdef CPPSIM_ENABLE_ASSERT
    #define CPPSIM_ASSERT(x, ...) {if (!(x)) {CPPSIM_ERROR("Assertion failed: {0} (In function {1}, line {2})", __VA_ARGS__, __func__, __LINE__); std::abort();}}
#else
    #define CPPSIM_ASSERT(x, ...)
#endif