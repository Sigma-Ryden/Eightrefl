# base
cmake_minimum_required(VERSION 3.5)

project(RewTesting LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# ~base

# packages
set(Automation_DIR ${CMAKE_CURRENT_LIST_DIR}/../Automation/cmake)
find_package(Automation REQUIRED)

set(Rew_DIR ${CMAKE_CURRENT_LIST_DIR}/cmake)
find_package(Rew REQUIRED)
# ~packages

# test
include_directories(test)

file(GLOB_RECURSE ${PROJECT_NAME}_TEST_SOURCES test/*.cpp)
file(GLOB_RECURSE ${PROJECT_NAME}_TEST_HEADERS test/*.hpp) # optionaly

add_compile_definitions(REW_FULLY_ENABLE)
add_compile_definitions(REW_DEV_ENABLE)

add_executable(${PROJECT_NAME}
    ${${PROJECT_NAME}_TEST_SOURCES}
    ${${PROJECT_NAME}_TEST_HEADERS} # optionaly
    ${Rew_HEADER_FILES} # optionaly
)
target_link_libraries(${PROJECT_NAME} Automation Rew)
# ~test
