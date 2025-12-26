cmake_minimum_required(VERSION 3.20)
project(loom_vm)

set(CMAKE_CXX_STANDARD 20)

# This tells the compiler to look in /include for your .hpp files
include_directories(include)

# Only list the files that need to be compiled (.cpp files)
add_executable(loom_vm 
    src/main.cpp
    src/LoomVM.cpp
)
