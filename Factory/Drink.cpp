#include <iostream>
#include <memory>
#include <map>
#include <functional>


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
        auto drink = hot_factories[drink_name]->make();
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

int main() {
    DrinkMachine machine;
    std::string tea = "tea";
    machine.makeDrink(tea);

    DrinkWithVolumeFactory factory;
    std::string coffee = "coffee";
    auto drink = factory.make_drink(coffee);
}
