cmake_minimum_required(VERSION 3.20)

project(wecs LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(wecs INTERFACE)
target_include_directories(wecs INTERFACE "./")
target_compile_features(wecs INTERFACE cxx_std_17)

option(WECS_BUILD_TEST "enable unittest" OFF)

if (PROJECT_IS_TOP_LEVEL OR WECS_BUILD_TEST)
    include(CTest)
    enable_testing()
    add_subdirectory(test)
endif()

option(WECS_BUILD_DEMO "build demo" OFF)

if (WECS_BUILD_DEMO)
    add_subdirectory(demo)
endif()