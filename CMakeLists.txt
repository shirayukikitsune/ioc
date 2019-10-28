cmake_minimum_required(VERSION 3.1)
project(KITSUNE_IOC VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(include)

if ("${CMAKE_SOURCE_DIR}" EQUAL "${CMAKE_CURRENT_SOURCE_DIR}")
    option(EnableTest "Enable unit testing" ON)

    if (EnableTest)
        enable_testing()
        add_subdirectory(test)
    endif()
endif()
