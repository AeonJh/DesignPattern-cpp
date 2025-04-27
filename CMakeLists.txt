cmake_minimum_required(VERSION 3.10)

#------------------------------------------------------------------------------
# Project configuration
#------------------------------------------------------------------------------
project(
    DesignPattern
    VERSION 0.1
    DESCRIPTION "Design Pattern Example"
    LANGUAGES CXX
)

file(GLOB SOLID_DIRS "SOLID/*")
foreach(dir ${SOLID_DIRS})
    if(IS_DIRECTORY ${dir})
        add_subdirectory(${dir})
    endif()
endforeach()

add_subdirectory(Builder)
add_subdirectory(Factory)
add_subdirectory(Mediator)
add_subdirectory(NullObject)
add_subdirectory(Prototype)
add_subdirectory(Singleton)
