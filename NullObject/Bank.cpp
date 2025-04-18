#include <iostream>
#include <string>
#include <memory>
#include "boost/lexical_cast.hpp"


struct Logger {
    virtual ~Logger() = default;
    virtual void info(const std::string& s) = 0;
    virtual void warn(const std::string& s) = 0;
};

struct ConsoleLogger : Logger
{
    void info(const std::string& s) override
    {
        std::cout << "INFO: " << s << std::endl;
    }
    void warn(const std::string& s) override
    {
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

int main() {
    auto logger = std::make_shared<ConsoleLogger>();
    BankAccount account_with_log{ "primary account 1", 1000, logger };
    BankAccount account_without_log{ "primary account 2", 1000};
    account_with_log.deposit(2000);
    account_without_log.deposit(2000); // no crash
}
