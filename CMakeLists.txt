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
set(EIGHTREFL_TESTS_LAUNCHER_NAME "Launcher")

if(PROJECT_IS_TOP_LEVEL)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/Binaries")
    if(APPLE)
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/Binaries/${EIGHTREFL_TESTS_LAUNCHER_NAME}.app/Contents/Frameworks")
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
add_library("${PROJECT_NAME}" ${PROJECT_LIBS_TYPE} ${PROJECT_SOURCES_FILES})
target_include_directories("${PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include")

if(EIGHTREFL_BUILD_SHARED_LIBS)
    set_target_properties("${PROJECT_NAME}" PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

if(EIGHTREFL_FULLY_ENABLE)
    target_compile_definitions("${PROJECT_NAME}" PUBLIC "EIGHTREFL_FULLY_ENABLE")
endif()
if(EIGHTREFL_RTTI_ALL_ENABLE)
    target_compile_definitions("${PROJECT_NAME}" PUBLIC "EIGHTREFL_RTTI_ALL_ENABLE")
endif()
if(EIGHTREFL_DEV_ENABLE)
    # we recomend in Release builds only
    target_compile_definitions("${PROJECT_NAME}" PUBLIC "EIGHTREFL_DEV_ENABLE")
endif()


# [[Testing]]
if(EIGHTREFL_BUILD_TEST_LIBS)
    file(GLOB_RECURSE PROJECT_TEST_SOURCES "${CMAKE_CURRENT_LIST_DIR}/test/*.cpp" "${CMAKE_CURRENT_LIST_DIR}/test/*.hpp")
    add_library("${PROJECT_NAME}Testing" ${PROJECT_LIBS_TYPE} ${PROJECT_TEST_SOURCES})

    if(PROJECT_IS_TOP_LEVEL)
        # you should manually download Eightest if not
        add_subdirectory("Eightest")
    endif()

    target_link_libraries("${PROJECT_NAME}Testing" PUBLIC "${PROJECT_NAME}" "Eightest")
    target_include_directories("${PROJECT_NAME}Testing" PRIVATE "${CMAKE_CURRENT_LIST_DIR}/test")
    set_target_properties("${PROJECT_NAME}Testing" PROPERTIES BUILD_WITH_INSTALL_RPATH TRUE INSTALL_RPATH "${EIGHTREFL_RPATH}")
endif()


# [[InplaceTesting]]
if(PROJECT_IS_TOP_LEVEL AND EIGHTREFL_BUILD_TEST_LIBS)
    set(EIGHTREFL_TESTS_LAUNCHER_CPP "${CMAKE_BINARY_DIR}/Launcher.cpp")
    file(WRITE "${EIGHTREFL_TESTS_LAUNCHER_CPP}"
        "#include <Eightest/Core.hpp>
        #if defined(_WIN32)
            #include <windows.h>
            #define EIGHTREFL_LOAD_MODULE(name) LoadLibraryA(#name\".dll\")
        #elif defined(__linux__)
            #include <dlfcn.h>
            #define EIGHTREFL_LOAD_MODULE(name) dlopen(#name\".so\", RTLD_NOW)
        #elif defined(__APPLE__)
            #include <dlfcn.h>
            #define EIGHTREFL_LOAD_MODULE(name) dlopen(#name\".dylib\", RTLD_NOW)
        #endif
        int main() {
            EIGHTREFL_LOAD_MODULE(EightreflTesting);
            TRY_CATCH(EXECUTE_ALL());
            return !TESTING_STAT();
        }"
    )

    add_executable("${EIGHTREFL_TESTS_LAUNCHER_NAME}" "${EIGHTREFL_TESTS_LAUNCHER_CPP}")

    target_link_libraries("${EIGHTREFL_TESTS_LAUNCHER_NAME}" PUBLIC "Eightest")    
    if(LINUX)
        target_link_libraries("${EIGHTREFL_TESTS_LAUNCHER_NAME}" PUBLIC "dl")
    endif()

    set_target_properties("${EIGHTREFL_TESTS_LAUNCHER_NAME}" PROPERTIES BUILD_WITH_INSTALL_RPATH TRUE INSTALL_RPATH "${EIGHTREFL_RPATH}")
    if(APPLE)
        set_target_properties("${EIGHTREFL_TESTS_LAUNCHER_NAME}" PROPERTIES MACOSX_BUNDLE TRUE)
    endif()
endif()
