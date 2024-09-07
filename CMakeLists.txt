cmake_minimum_required(VERSION 3.5)

project(Rew LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE REW_HEADERS include/Rew/*.hpp)
file(GLOB_RECURSE AUTO_TESTING_SOURCES include/AutoTesting/*.cpp)
file(GLOB_RECURSE REW_TESTING_SOURCES test/*.cpp)

add_executable(${PROJECT_NAME} ${REW_HEADERS} ${AUTO_TESTING_SOURCES} ${REW_TESTING_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC include PRIVATE test)
