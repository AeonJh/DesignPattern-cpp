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
- [ ] Adapter
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
- [ ] Strategy
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
   ```

2. Navigate to each individual Design Patterns folder, then:
   ```bash
   cmake -B build -S .
   ```

3. Build the project:
   ```bash
   cmake --build build
   ```

4. Run the compiled examples:
   Each pattern's executable is located in the `build/bin` directory. (e.g., `./build/bin/person`)

## License
This repository is licensed under the MIT License. See the [LICENSE](LICENSE) file for more details.

---

Feel free to modify this README to suit your preferences or add any additional information you'd like! Let me know if you'd like help with specific sections.
