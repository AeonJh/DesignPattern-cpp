cmake_minimum_required(VERSION 3.10)

#------------------------------------------------------------------------------
# Project configuration
#------------------------------------------------------------------------------
project(
    BridgePattern
    VERSION 0.1
    DESCRIPTION "Bridge Design Pattern Example"
    LANGUAGES CXX
)

#------------------------------------------------------------------------------
# Compiler configuration
#------------------------------------------------------------------------------
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -Werror")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

#------------------------------------------------------------------------------
# Source files configuration
#------------------------------------------------------------------------------
add_executable(bridge main.cpp)

#------------------------------------------------------------------------------
# gtest headers and libraries configuration
#------------------------------------------------------------------------------
# Link gtest headers
target_include_directories(bridge PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../external/gtest/googletest/include)
# Link gtest libraries  
target_link_libraries(bridge PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../external/gtest/build/lib/libgtest.a
    ${CMAKE_CURRENT_SOURCE_DIR}/../external/gtest/build/lib/libgtest_main.a
    pthread
)
