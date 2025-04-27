#include <iostream>
#include <memory>
#include <map>
#include <functional>
#include <gtest/gtest.h>


struct HotDrink {
    virtual void prepare(int volume) = 0;
};

struct Tea: HotDrink {
    void prepare(int volume) override {
        std::cout << "Take tea bag, boil water, pour "
                  << volume
                  << "ml, add some lemon." << std::endl;
    }
};

struct Coffee: HotDrink {
    void prepare(int volume) override {
        std::cout << "Take coffee,  boil water, pour "
                  << volume
                  << "ml, add sugar and milk." << std::endl;
    }
};

struct HotDrinkFactory {
    virtual std::unique_ptr<HotDrink> make() = 0;
};

struct TeaFactory: HotDrinkFactory {
    std::unique_ptr<HotDrink> make() override {
        return std::make_unique<Tea>();
    }
};

struct CoffeeFactory: HotDrinkFactory {
    std::unique_ptr<HotDrink> make() override {
        return std::make_unique<Coffee>();
    }
};

struct DrinkMachine {
    std::map<std::string, std::unique_ptr<HotDrinkFactory>> hot_factories;

    std::unique_ptr<HotDrink> makeDrink(const std::string& drink_name) {
        auto it = hot_factories.find(drink_name);
        if (it == hot_factories.end()) throw std::out_of_range("Invalid drink type");
        auto drink = it->second->make();
        drink->prepare(200);
        return drink;
    }

    DrinkMachine() {
        hot_factories["tea"] = std::make_unique<TeaFactory>();
        hot_factories["coffee"] = std::make_unique<CoffeeFactory>();
    }
};

// Fuction factory
class DrinkWithVolumeFactory {
    std::map<std::string, std::function<std::unique_ptr<HotDrink>()>> hot_factories;
public:
    DrinkWithVolumeFactory() {
        hot_factories["tea"] = []() {
            auto tea = std::make_unique<Tea>();
            tea->prepare(200);
            return tea;
        };
        hot_factories["coffee"] = []() {
            auto coffee = std::make_unique<Coffee>();
            coffee->prepare(200);
            return coffee;
        };
    }
    std::unique_ptr<HotDrink> make_drink(const std::string &name) const;
};

inline std::unique_ptr<HotDrink> DrinkWithVolumeFactory::make_drink(const std::string &name) const {
    return hot_factories.at(name)();
}

class DrinkTest : public ::testing::Test {
protected:
    // Redirect cout to our stringstream
    void SetUp() override {
        oldCoutBuf = std::cout.rdbuf(output.rdbuf());
    }

    // Restore cout
    void TearDown() override {
        std::cout.rdbuf(oldCoutBuf);
    }

    std::stringstream output;
    std::streambuf* oldCoutBuf;
};

TEST_F(DrinkTest, TeaMaking) {
    DrinkMachine machine;
    machine.makeDrink("tea");
    EXPECT_EQ(output.str(), "Take tea bag, boil water, pour 200ml, add some lemon.\n");
}

TEST_F(DrinkTest, CoffeeMaking) {
    DrinkMachine machine;
    machine.makeDrink("coffee");
    EXPECT_EQ(output.str(), "Take coffee,  boil water, pour 200ml, add sugar and milk.\n");
}

TEST_F(DrinkTest, DrinkWithVolumeFactoryTea) {
    DrinkWithVolumeFactory factory;
    factory.make_drink("tea");
    EXPECT_EQ(output.str(), "Take tea bag, boil water, pour 200ml, add some lemon.\n");
}

TEST_F(DrinkTest, DrinkWithVolumeFactoryCoffee) {
    DrinkWithVolumeFactory factory;
    factory.make_drink("coffee");
    EXPECT_EQ(output.str(), "Take coffee,  boil water, pour 200ml, add sugar and milk.\n");
}

TEST_F(DrinkTest, InvalidDrinkType) {
    DrinkMachine machine;
    EXPECT_THROW(machine.makeDrink("juice"), std::out_of_range);
}

TEST_F(DrinkTest, InvalidDrinkTypeInVolumeFactory) {
    DrinkWithVolumeFactory factory;
    EXPECT_THROW(factory.make_drink("juice"), std::out_of_range);
}

TEST_F(DrinkTest, TeaPreparation) {
    auto tea = std::make_unique<Tea>();
    tea->prepare(150);
    EXPECT_EQ(output.str(), "Take tea bag, boil water, pour 150ml, add some lemon.\n");
}

TEST_F(DrinkTest, CoffeePreparation) {
    auto coffee = std::make_unique<Coffee>();
    coffee->prepare(300);
    EXPECT_EQ(output.str(), "Take coffee,  boil water, pour 300ml, add sugar and milk.\n");
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
