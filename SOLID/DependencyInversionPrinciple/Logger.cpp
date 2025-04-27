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
#include <gtest/gtest.h>


struct ILogger {
    virtual ~ILogger() { }
    virtual void Log(const std::string& s) = 0;
    virtual void log_info(const std::string& s) = 0;
};

struct ConsoleLogger : ILogger {
    ConsoleLogger() { }

    void Log(const std::string& s) override {
        std::cout << "LOG: " << s.c_str() << std::endl;
    }
    
    void log_info(const std::string& s) override {
        std::cout << "INFO: " << s.c_str() << std::endl;
    }
};

class Reporting {
    ILogger& logger;
public:
    Reporting(ILogger& logger) : logger{logger} { }
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

class LoggerTest : public ::testing::Test {
protected:
    void SetUp() override {
        oldCoutBuf = std::cout.rdbuf(output.rdbuf());
    }

    void TearDown() override {
        std::cout.rdbuf(oldCoutBuf);
    }

    std::stringstream output;
    std::streambuf* oldCoutBuf;
};

class MockLogger : public ILogger {
public:
    std::vector<std::string> log_messages;
    std::vector<std::string> info_messages;

    void Log(const std::string& s) override {
        log_messages.push_back(s);
    }

    void log_info(const std::string& s) override {
        info_messages.push_back(s);
    }
};

TEST_F(LoggerTest, ConsoleLoggerLog) {
    ConsoleLogger logger;
    logger.Log("Test message");
    EXPECT_EQ(output.str(), "LOG: Test message\n");
}

TEST_F(LoggerTest, ConsoleLoggerInfo) {
    ConsoleLogger logger;
    logger.log_info("Info message");
    EXPECT_EQ(output.str(), "INFO: Info message\n");
}

TEST_F(LoggerTest, ReportingWithLogger) {
    MockLogger logger;
    Reporting reporting(logger);
    reporting.prepare_report();
    ASSERT_EQ(logger.info_messages.size(), size_t(1));
    EXPECT_EQ(logger.info_messages[0], "Preparing the report");
}

TEST_F(LoggerTest, CarWithLogger) {
    auto logger = std::make_shared<MockLogger>();
    auto engine = std::make_unique<Engine>();
    Car car(std::move(engine), logger);

    ASSERT_EQ(logger->log_messages.size(), size_t(1));
    EXPECT_EQ(logger->log_messages[0], "making a car");
}

TEST_F(LoggerTest, CarEngineToString) {
    auto logger = std::make_shared<ConsoleLogger>();
    auto engine = std::make_unique<Engine>();
    engine->volume = 2.0f;
    engine->horse_power = 200;

    std::stringstream ss;
    ss << *engine;
    EXPECT_EQ(ss.str(), "volume: 2, horse_power: 200");
}

TEST_F(LoggerTest, CarToString) {
    auto logger = std::make_shared<ConsoleLogger>();
    auto engine = std::make_unique<Engine>();
    engine->volume = 3.0f;
    engine->horse_power = 300;
    Car car(std::move(engine), logger);

    std::stringstream ss;
    ss << car;
    EXPECT_EQ(ss.str(), "car with engine: volume: 3, horse_power: 300");
}

TEST_F(LoggerTest, DIContainerTest) {
    auto injector = boost::di::make_injector(
        boost::di::bind<ILogger>().to<ConsoleLogger>()
    );

    auto car = injector.create<std::shared_ptr<Car>>();
    EXPECT_NE(car, nullptr);
    EXPECT_NE(car->logger, nullptr);
}

TEST_F(LoggerTest, CompleteWorkflow) {
    auto injector = boost::di::make_injector(
        boost::di::bind<ILogger>().to<ConsoleLogger>()
    );

    auto car = injector.create<std::shared_ptr<Car>>();
    auto engine = std::make_unique<Engine>();
    car->engine = std::move(engine);

    Reporting report(*car->logger);
    report.prepare_report();

    EXPECT_TRUE(output.str().find("making a car") == std::string::npos); // Only MockLogger should log output
    EXPECT_TRUE(output.str().find("INFO: Preparing the report") != std::string::npos);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
