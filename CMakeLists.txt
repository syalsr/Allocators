cmake_minimum_required(VERSION 3.22)
project(Allocators)

set(CMAKE_CXX_STANDARD 17)

add_executable(Allocators main.cpp LinearAllocator.hpp StackAllocator.hpp SimpleTemplateAllocator.hpp)

