cmake_minimum_required(VERSION 3.10)

#------------------------------------------------------------------------------
# Project configuration
#------------------------------------------------------------------------------
project(
    Builder
    VERSION 0.1
    DESCRIPTION "Builder Pattern Example"
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
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -Werror")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

#------------------------------------------------------------------------------
# Source files configuration - 1
#------------------------------------------------------------------------------
add_executable(simple_web_page simple_web_page.cpp)

#------------------------------------------------------------------------------
# gtest headers and libraries configuration - 1
#------------------------------------------------------------------------------
# Link gtest headers
target_include_directories(simple_web_page PRIVATE $ENV{GTEST_INC})
# Link gtest libraries
target_link_libraries(simple_web_page PRIVATE
    $ENV{GTEST_LIB}/libgtest.a
    $ENV{GTEST_LIB}/libgtest_main.a
)

# Link pthread libraries (needed for gtest)
target_link_libraries(simple_web_page PRIVATE
    pthread
)

#------------------------------------------------------------------------------
# Source files configuration - 2
#------------------------------------------------------------------------------
add_executable(person person.cpp)

#------------------------------------------------------------------------------
# gtest headers and libraries configuration - 2
#------------------------------------------------------------------------------
# Link gtest headers
target_include_directories(person PRIVATE $ENV{GTEST_INC})
# Link gtest libraries
target_link_libraries(person PRIVATE
    $ENV{GTEST_LIB}/libgtest.a
    $ENV{GTEST_LIB}/libgtest_main.a
)

# Link pthread libraries (needed for gtest)
target_link_libraries(person PRIVATE
    pthread
)
