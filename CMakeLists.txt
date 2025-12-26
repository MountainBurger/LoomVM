cmake_minimum_required(VERSION 4.1)
project(loom_vm)

set(CMAKE_CXX_STANDARD 20)

add_executable(loom_vm main.cpp
        LoomVM.hpp
)
