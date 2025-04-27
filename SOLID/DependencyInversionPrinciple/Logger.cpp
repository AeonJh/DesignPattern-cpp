/**
 * @file Logger.cpp
 * 
 * @brief Example of Dependency Inversion Principle (DIP)
 * 
 * @details The code demonstrates the Dependency Inversion Principle (DIP).
 *
 * @Dependency Inversion Principle (DIP): 
 * A. High-level modules should not depend on low-level modules. Both should depend on abstractions.
 * B. Abstractions should not depend on details. Details should depend on abstractions.
 *
 * @author AeonJh
 * @date 2025-04-09
 */


#include <iostream>
#include <memory>
#include "di.hpp"


struct ILogger {
    virtual ~ILogger() { }
    virtual void Log(const std::string& s) = 0;
    virtual void log_info(const std::string& s) const = 0;
};

struct ConsoleLogger : ILogger {
    ConsoleLogger() { }

    void Log(const std::string& s) override {
        std::cout << "LOG: " << s.c_str() << std::endl;
    }
    
    void log_info(const std::string& s) const override {
        std::cout << "INFO: " << s.c_str() << std::endl;
    }
};

class Reporting {
    const ILogger& logger;
public:
    Reporting(const ILogger& logger) : logger{logger} { }
    void prepare_report() {
        logger.log_info("Preparing the report");
    }
};

struct Engine {
    float volume = 5;
    int horse_power = 400;
    friend std::ostream& operator<< (std::ostream& os, const Engine& obj) {
        return os << "volume: " << obj.volume
            << ", horse_power: " << obj.horse_power;
    }
};

struct Car {
    std::unique_ptr<Engine> engine;
    std::shared_ptr<ILogger> logger;
    Car(std::unique_ptr<Engine> engine, const std::shared_ptr<ILogger>& logger)
      : engine{std::move(engine)}, logger{logger} {
        logger->Log("making a car");
    }

    friend std::ostream& operator<<(std::ostream& os, const Car& obj) {
        return os << "car with engine: " << *obj.engine;
    }
};


int main() {
    auto injector = boost::di::make_injector(boost::di::bind<ILogger>().to<ConsoleLogger>());
    auto car = injector.create<std::shared_ptr<Car>>();
    auto engine = std::make_unique<Engine>();
    car->engine = std::move(engine);
    std::cout << *car << std::endl;
    Reporting report(*car->logger);
    report.prepare_report();

    return 0;
}
