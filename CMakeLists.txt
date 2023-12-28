cmake_minimum_required(VERSION 3.20...3.28)
include(FetchContent)
project(lox)

message(STATUS "CMake ${CMAKE_VERSION}")
enable_testing()
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++20")

set(CMAKE_CXX_FLAGS_INIT                "-Wall -std=c++23")
set(CMAKE_CXX_FLAGS_DEBUG_INIT          "-g")
set(CMAKE_CXX_FLAGS_MINSIZEREL_INIT     "-Os -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE_INIT        "-O3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO_INIT "-O2 -g")

file(GLOB_RECURSE SOURCE_LIST "${CMAKE_CURRENT_LIST_DIR}/src/*.cpp")
file(GLOB_RECURSE HEADER_LIST "${CMAKE_CURRENT_LIST_DIR}/include/lox/*.hpp")

add_library(${PROJECT_NAME} ${SOURCE_LIST} ${HEADER_LIST})
add_subdirectory(tests)

target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    PRIVATE
    src)