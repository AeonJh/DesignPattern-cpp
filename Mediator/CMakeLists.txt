cmake_minimum_required(VERSION 3.10)

#------------------------------------------------------------------------------
# Project configuration
#------------------------------------------------------------------------------
project(
    Mediator
    VERSION 0.1
    DESCRIPTION "Mediator Pattern Example"
    LANGUAGES CXX
)

#------------------------------------------------------------------------------
# Boost configuration
#------------------------------------------------------------------------------
if(NOT DEFINED ENV{BOOST_ROOT})
    message(FATAL_ERROR "Please set the BOOST_ROOT environment variable to the Boost installation directory.\n"
        "Example: export BOOST_ROOT=/path/to/boost/build/include")
endif()
set(Boost_NO_SYSTEM_PATHS ON)  # Force CMake to ignore system Boost
set(Boost_USE_STATIC_LIBS OFF)
# set(Boost_DEBUG ON)

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
add_executable(football Football.cpp)

#------------------------------------------------------------------------------
# Boost headers and libraries configuration
#------------------------------------------------------------------------------
# Link Boost headers
target_include_directories(football PRIVATE $ENV{BOOST_ROOT})

#------------------------------------------------------------------------------
# gtest headers and libraries configuration - 1
#------------------------------------------------------------------------------
# Link gtest headers
target_include_directories(football PRIVATE $ENV{GTEST_INC})
# Link gtest libraries
target_link_libraries(football PRIVATE
    $ENV{GTEST_LIB}/libgtest.a
    $ENV{GTEST_LIB}/libgtest_main.a
)

# Link pthread libraries (needed for gtest)
target_link_libraries(football PRIVATE
    pthread
)

#------------------------------------------------------------------------------
# Source files configuration - 2
#------------------------------------------------------------------------------
add_executable(chatroom Chatroom.cpp)

#------------------------------------------------------------------------------
# gtest headers and libraries configuration - 2
#------------------------------------------------------------------------------
# Link gtest headers
target_include_directories(chatroom PRIVATE $ENV{GTEST_INC})
# Link gtest libraries
target_link_libraries(chatroom PRIVATE
    $ENV{GTEST_LIB}/libgtest.a
    $ENV{GTEST_LIB}/libgtest_main.a
)

# Link pthread libraries (needed for gtest)
target_link_libraries(chatroom PRIVATE
    pthread
)
