# Design Patterns in Modern C++

This repository contains the source code I created while learning from the book **"Design Pattern in Modern C++"**. It serves as a practical implementation of various design patterns using Modern C++ (C++11 and beyond). Each design pattern is implemented in a separate module, demonstrating its principles in an easy-to-understand way.

## Table of Contents
- [Introduction](#introduction)
- [Design Patterns Covered](#design-patterns-covered)
- [Setup and Requirements](#setup-and-requirements)
- [License](#license)

## Introduction
Design patterns are proven solutions to recurring problems in software design. They provide a standardized approach to solving common engineering challenges, making code more reusable, maintainable, and scalable.

This repository explores these patterns with Modern C++, leveraging features like smart pointers, lambda functions, and more, making the implementations concise and idiomatic.

*NOTE: You need to download the boost library and build some libraries like: Boost.Serialization.*

## Design Patterns Covered
Here are the categories of design patterns implemented:

### Creational Patterns
- [x] Singleton
- [x] Factory Method
- [x] Abstract Factory
- [x] Builder
- [x] Prototype

### Structural Patterns
- [x] Adapter
- [ ] Bridge
- [ ] Composite
- [ ] Decorator
- [ ] Facade
- [ ] Flyweight
- [ ] Proxy

### Behavioral Patterns
- [ ] Chain of Responsibility
- [ ] Command
- [ ] Interpreter
- [ ] Iterator
- [x] Mediator
- [ ] Memento
- [x] Null Object
- [ ] Observer
- [ ] State
- [x] Strategy
- [ ] Template Method
- [ ] Visitor

## Setup and Requirements
### Prerequisites
To run the examples in this repository, you'll need:
- A C++ compiler that supports C++11 or newer (e.g., GCC, Clang).
- CMake (version 3.10 or newer) for building the project.

### Build Instructions
1. Clone the repository:
   ```bash
   git clone https://github.com/AeonJh/DesignPattern-cpp.git
   cd DesignPattern-cpp

   # set current directory as `PROJECT_ROOT`
   export PROJECT_ROOT=$PWD
   ```

2. Pull and update submodule (boost & gtest): (This might take some time)
   ```bash
   git submodule update --init --recursive
   ```

3. Compile and install libraries (boost & gtest): (recommended)
   ```bash
   # boost
   cd external/boost
   ./bootstrap.sh --prefix=build
   ./b2 install

   # return to project directory
   cd ..

   # gtest
   cd external/gtest
   # only compile googletest, no gmock
   cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=on \
         -DCMAKE_BUILD_TYPE=Debug \
         -DBUILD_GMOCK=OFF \
         -B build -S .
   cmake --build build
   cmake --install build --strip --prefix build
   ```

5. For needed Design Pattern examples, export the dependency include path and libraries path: (optional)
   ```bash
   export BOOST_EXT_ROOT=$PROJECT_ROOT/external/boost-ext
   export BOOST_ROOT=$PROJECT_ROOT/external/boost/build/include
   export BOOST_LIB=$PROJECT_ROOT/external/boost/build/lib
   export GTEST_INC=$PROJECT_ROOT/external/gtest/build/include
   export GTEST_LIB=$PROJECT_ROOT/external/gtest/build/lib
   ```

5. Navigate to each individual Design Patterns folder, build and run it, e.g.:
   ```bash
   cd Singleton
   cmake -B build -S .
   cmake --build build
   # Each pattern's executable is located in the `build/bin` directory.
   ./build/bin/database

   # Or you can compile all the target at the PROJECT_ROOT directory,
   # then you can find all the executable file in ./build/bin.
   cd $PROJECT_ROOT
   ./build.sh

   # When build all the target, you can run the check.sh to test.
   ./check.sh
   ```

## License
This repository is licensed under the MIT License. See the [LICENSE](LICENSE) file for more details.

---

Feel free to modify this README to suit your preferences or add any additional information you'd like! Let me know if you'd like help with specific sections.
