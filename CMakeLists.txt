# [[Root]]
cmake_minimum_required(VERSION 3.5)

project(Eightrefl)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_SHARED_LIBRARY_PREFIX "")


# [[Options]]
option(EIGHTREFL_BUILD_SHARED_LIBS "Build shared libraies by Default" ON)
option(EIGHTREFL_FULLY_ENABLE "Build by Default" OFF)
option(EIGHTREFL_RTTI_ALL_ENABLE "Build by Default" OFF)
option(EIGHTREFL_DEV_ENABLE "Build by Default" OFF)
option(EIGHTREFL_BUILD_TEST_LIBS "Build testing libraies by Default" OFF)


# [[Defaults]]
if(PROJECT_IS_TOP_LEVEL)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/Binaries")
    if(APPLE)
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/Binaries/EightestLauncher.app/Contents/Frameworks")
    else()
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/Binaries")
    endif()
endif()

if(LINUX)
    set(EIGHTREFL_RPATH "$ORIGIN")
elseif(APPLE)
    set(EIGHTREFL_RPATH "@loader_path/../Frameworks")
endif()


# [[Binaries]]
if(EIGHTREFL_BUILD_SHARED_LIBS)
    set(PROJECT_LIBS_TYPE SHARED)
else()
    set(PROJECT_LIBS_TYPE STATIC)
    message(WARNING "Eightrefl: build static library not recommended, registration may be lost, use export all symbols to be sure")
endif()


file(GLOB_RECURSE PROJECT_SOURCES_FILES "${CMAKE_CURRENT_LIST_DIR}/src/*.cpp" "${CMAKE_CURRENT_LIST_DIR}/include/*.hpp")
add_library(Eightrefl ${PROJECT_LIBS_TYPE} ${PROJECT_SOURCES_FILES})
target_include_directories(Eightrefl PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include")

if(EIGHTREFL_BUILD_SHARED_LIBS)
    set_target_properties(Eightrefl PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
endif()

if(EIGHTREFL_FULLY_ENABLE)
    target_compile_definitions(Eightrefl PUBLIC "EIGHTREFL_FULLY_ENABLE")
endif()
if(EIGHTREFL_RTTI_ALL_ENABLE)
    target_compile_definitions(Eightrefl PUBLIC "EIGHTREFL_RTTI_ALL_ENABLE")
endif()
if(EIGHTREFL_DEV_ENABLE)
    # we recomend in Release builds only
    target_compile_definitions(Eightrefl PUBLIC "EIGHTREFL_DEV_ENABLE")
endif()


# [[Tests]]
if(EIGHTREFL_BUILD_TEST_LIBS)
    file(GLOB_RECURSE PROJECT_TEST_SOURCES "${CMAKE_CURRENT_LIST_DIR}/test/*.cpp" "${CMAKE_CURRENT_LIST_DIR}/test/*.hpp")
    add_library(EightreflTests ${PROJECT_LIBS_TYPE} ${PROJECT_TEST_SOURCES})

    if(PROJECT_IS_TOP_LEVEL)
        # you should manually download Eightest if not
        set(EIGHTEST_RUN_MODULE "EightreflTests")
        add_subdirectory("Eightest")
    endif()

    target_link_libraries(EightreflTests PUBLIC Eightrefl Eightest)
    target_include_directories(EightreflTests PRIVATE "${CMAKE_CURRENT_LIST_DIR}/test")
    set_target_properties(EightreflTests PROPERTIES BUILD_WITH_INSTALL_RPATH TRUE INSTALL_RPATH "${EIGHTREFL_RPATH}")
endif()
