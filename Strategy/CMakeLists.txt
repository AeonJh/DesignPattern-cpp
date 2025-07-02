cmake_minimum_required(VERSION 3.10)

#------------------------------------------------------------------------------
# Project configuration
#------------------------------------------------------------------------------
project(
    Strategy
    VERSION 0.1
    DESCRIPTION "Strategy Pattern Example"
    LANGUAGES CXX
)

#------------------------------------------------------------------------------
# gtest configuration
#------------------------------------------------------------------------------
if (NOT DEFINED ENV{GTEST_INC})
    message(FATAL_ERROR "Please set the GTEST_INC environment variable to the gtest include directory.\n"
        "Example: export GTEST_INC=/path/to/gtest/include")
endif()
if (NOT DEFINED ENV{GTEST_LIB})
    message(FATAL_ERROR "Please set the GTEST_LIB environment variable to the gtest library directory.\n"
        "Example: export GTEST_LIB=/path/to/gtest/lib")
endif()

#------------------------------------------------------------------------------
# Compiler configuration
#------------------------------------------------------------------------------
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -Werror -Wno-unused-parameter")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

#------------------------------------------------------------------------------
# Source files configuration
#------------------------------------------------------------------------------
add_executable(a.out main.cpp)

#------------------------------------------------------------------------------
# gtest headers and libraries configuration
#------------------------------------------------------------------------------
# Link gtest headers
target_include_directories(a.out PRIVATE $ENV{GTEST_INC})
# Link gtest libraries
target_link_libraries(a.out PRIVATE
    $ENV{GTEST_LIB}/libgtest.a
    $ENV{GTEST_LIB}/libgtest_main.a
)
