cmake_minimum_required(VERSION 3.5)

project(Rew LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(Rew include/Rew/Rew.hpp test/main.cpp)
