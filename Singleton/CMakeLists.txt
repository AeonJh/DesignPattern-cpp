cmake_minimum_required(VERSION 3.10)

#------------------------------------------------------------------------------
# Project configuration
#------------------------------------------------------------------------------
project(
    Singleton
    VERSION 0.1
    DESCRIPTION "Singleton Pattern Example"
    LANGUAGES CXX
)

#------------------------------------------------------------------------------
# Boost configuration
#------------------------------------------------------------------------------
if(NOT DEFINED ENV{BOOST_ROOT})
    message(FATAL_ERROR "Please set the BOOST_ROOT environment variable to the Boost installation directory.\n"
        "Example: export BOOST_ROOT=/path/to/boost_1_XX_0")
endif()
if(NOT DEFINED ENV{BOOST_LIB})
    message(FATAL_ERROR "Please set the BOOST_LIB environment variable to the Boost library name.\n"
        "Example: export BOOST_LIB=/path/to/boost/stage/lib")
endif()
set(Boost_NO_SYSTEM_PATHS ON)  # Force CMake to ignore system Boost
set(Boost_USE_STATIC_LIBS OFF)
# set(Boost_DEBUG ON)

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
add_executable(database Database.cpp)

#------------------------------------------------------------------------------
# Boost headers and libraries configuration
#------------------------------------------------------------------------------
# Link Boost headers
target_include_directories(database PRIVATE $ENV{BOOST_ROOT})
# Link Boost libraries
# target_link_libraries(a.out PRIVATE
#     $ENV{BOOST_LIB}/libboost_xxx.a
# )
