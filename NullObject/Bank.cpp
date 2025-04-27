#include <iostream>
#include <string>
#include <memory>
#include "boost/lexical_cast.hpp"
#include <gtest/gtest.h>


struct Logger {
    virtual ~Logger() = default;
    virtual void info(const std::string& s) = 0;
    virtual void warn(const std::string& s) = 0;
};

struct ConsoleLogger : Logger
{
    void info(const std::string& s) override {
        std::cout << "INFO: " << s << std::endl;
    }
    void warn(const std::string& s) override {
        std::cout << "WARNNING!!!" << s << std::endl;
    }
};

// Pimpl
struct OptionalLogger : Logger {
    std::shared_ptr<Logger> impl;
    static std::shared_ptr<Logger> no_logging;
    OptionalLogger(const std::shared_ptr<Logger>& logger) : impl { logger } { };
    virtual void info(const std::string& s) override {
        if (impl) impl->info(s); // null check
    }
    virtual void warn(const std::string& s) override {
        if (impl) impl->warn(s); // null check
    }
};

std::shared_ptr<Logger> OptionalLogger::no_logging{};

struct BankAccount {
    std::string name;
    int balance = 0;
    std::shared_ptr<OptionalLogger> logger;
    BankAccount(const std::string& name, int balance, const std::shared_ptr<Logger>& logger = OptionalLogger::no_logging)
        : name{ name },
          balance{ balance },
          logger { std::make_shared<OptionalLogger>(logger) } { };
    void deposit(int amount);
};

void BankAccount::deposit(int amount) {
    balance += amount;
    logger->info("Deposited $" + boost::lexical_cast<std::string>(amount)
                + " to " + name + ", balance is now $"
                + boost::lexical_cast<std::string>(balance));
}

class BankTest : public ::testing::Test {
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

TEST_F(BankTest, DepositWithLogging) {
    auto logger = std::make_shared<ConsoleLogger>();
    BankAccount account{"John Doe", 1000, logger};

    account.deposit(500);

    EXPECT_EQ(account.balance, 1500);
    EXPECT_EQ(output.str(), "INFO: Deposited $500 to John Doe, balance is now $1500\n");
}

TEST_F(BankTest, DepositWithoutLogging) {
    BankAccount account{"John Doe", 1000};

    account.deposit(500);

    EXPECT_EQ(account.balance, 1500);
    EXPECT_EQ(output.str(), ""); // No logging output expected
}

TEST_F(BankTest, ConsoleLoggerInfo) {
    auto logger = std::make_shared<ConsoleLogger>();
    logger->info("Test message");

    EXPECT_EQ(output.str(), "INFO: Test message\n");
}

TEST_F(BankTest, ConsoleLoggerWarn) {
    auto logger = std::make_shared<ConsoleLogger>();
    logger->warn("Warning message");

    EXPECT_EQ(output.str(), "WARNNING!!!Warning message\n");
}

TEST_F(BankTest, OptionalLoggerWithImpl) {
    auto consoleLogger = std::make_shared<ConsoleLogger>();
    auto logger = std::make_shared<OptionalLogger>(consoleLogger);

    logger->info("Test message");

    EXPECT_EQ(output.str(), "INFO: Test message\n");
}

TEST_F(BankTest, OptionalLoggerWithoutImpl) {
    auto logger = std::make_shared<OptionalLogger>(nullptr);

    logger->info("Test message");
    logger->warn("Warning message");

    EXPECT_EQ(output.str(), ""); // No output expected
}

TEST_F(BankTest, MultipleBankAccounts) {
    auto logger = std::make_shared<ConsoleLogger>();
    BankAccount account1{"John Doe", 1000, logger};
    BankAccount account2{"Jane Doe", 2000}; // No logger

    account1.deposit(500);
    account2.deposit(1000);

    EXPECT_EQ(account1.balance, 1500);
    EXPECT_EQ(account2.balance, 3000);
    EXPECT_EQ(output.str(), "INFO: Deposited $500 to John Doe, balance is now $1500\n");
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
