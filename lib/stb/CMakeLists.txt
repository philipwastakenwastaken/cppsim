cmake_minimum_required(VERSION 3.0)
project(stb)

add_library(stb INTERFACE)
target_include_directories(stb SYSTEM INTERFACE .)