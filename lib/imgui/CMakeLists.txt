cmake_minimum_required(VERSION 3.0)
project(imgui)

set(CMAKE_CXX_STANDARD 20)

add_library(imgui STATIC
                  imgui/imgui.cpp
                  imgui/imgui_draw.cpp
                  imgui/imgui_impl_glfw.cpp
                  imgui/imgui_demo.cpp
                  imgui/imgui_widgets.cpp
                  imgui/imgui_impl_opengl3.cpp
                  imgui/imgui_tables.cpp
                  imgui/imgui_impl_opengl3.h
                  imgui/imgui_impl_opengl3_loader.h
                  imgui/imgui_impl_glfw.h
                  imgui/imgui_internal.h
                  imgui/imgui.h
                  imgui/imconfig.h
                  imgui/imstb_rectpack.h
                  imgui/imstb_textedit.h
                  imgui/imstb_truetype.h
                  )
target_include_directories(imgui PUBLIC ../glfw/include imgui/)