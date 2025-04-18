cmake_minimum_required(VERSION 3.10)

project(
    Logger
    VERSION 0.1
    DESCRIPTION "Logger"
    LANGUAGES CXX
)

if(NOT DEFINED ENV{BOOST_EXT_DI_ROOT})
    message(FATAL_ERROR "Please set the BOOST_EXT_DI_ROOT environment variable to the Boost installation directory.\n"
        "Example: export BOOST_EXT_DI_ROOT=/path/to/di/include")
endif()
set(BOOST_EXT_DI_ROOT $ENV{BOOST_EXT_DI_ROOT})
# set(Boost_NO_SYSTEM_PATHS ON)  # Force CMake to ignore system Boost
# set(Boost_USE_STATIC_LIBS OFF)
# set(Boost_DEBUG ON)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions -fno-rtti -Wall -Wextra -pedantic-errors -Werror")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(logger Logger.cpp)

# Link Boost headers
message(STATUS "BOOST_EXT_DI_ROOT: ${BOOST_EXT_DI_ROOT}")
target_include_directories(logger PRIVATE ${BOOST_EXT_DI_ROOT})
