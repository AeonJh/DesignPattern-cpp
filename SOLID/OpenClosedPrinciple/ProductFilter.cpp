/** 
 * @file ProductFilter.cpp
 * 
 * @brief Example of Open-Closed Principle violation
 * 
 * @details This code demonstrates a violation of the Open-Closed Principle (OCP)
 * by using a filter class (OlderProductFilter) that requires modification to add new filtering criteria.
 * The BetterFilter class, on the other hand, adheres to the OCP
 * by allowing new filtering criteria to be added without modifying existing code.
 * 
 * @Open-Closed Principle: Software entities (classes, modules, functions, etc.) should be open for extension but closed for modification.
 * 
 * @author AeonJh
 * @date 2025-04-09
 */


#include <iostream>
#include <string>
#include <vector>


enum class Color { Red, Green, Blue };
enum class Size { Small, Medium, Large };

template <typename T> struct AndSpecification; // Forward declaration

template <typename T> struct Specification {
    virtual bool is_satisfied(T* item) const = 0;

    AndSpecification<T> operator&&(Specification<T>&& other) {
        return AndSpecification<T>(*this, other);
    }
};


template <typename T> struct Filter {
    virtual std::vector<T*> filter(std::vector<T*> items, const Specification<T>& spec) = 0;
};

template <typename T> struct AndSpecification : Specification<T> {
    const Specification<T>& first;
    const Specification<T>& second;

    AndSpecification(Specification<T>& first, Specification<T>& second)
        : first(first), second(second) {}

    AndSpecification(const Specification<T>&& first, const Specification<T>&& second)
        : first(first), second(second) {}

    bool is_satisfied(T* item) const override {
        return first.is_satisfied(item) && second.is_satisfied(item);
    }
};

struct Product {
    std::string name;
    Color color;
    Size size;
};

struct OlderProductFilter {
    typedef std::vector<Product*> Items;
    Items filter_by_color(Items items, Color color) {
        Items result;
        for (auto& item : items) {
            if (item->color == color) {
                result.push_back(item);
            }
        }
        return result;
    }

    Items filter_by_size(Items items, Size size) {
        Items result;
        for (auto& item : items) {
            if (item->size == size) {
                result.push_back(item);
            }
        }
        return result;
    }
};

struct BetterFilter : Filter<Product> {
    std::vector<Product*> filter(std::vector<Product*> items, const Specification<Product>& spec) override {
        std::vector<Product*> result;
        for (auto& item : items) {
            if (spec.is_satisfied(item)) {
                result.push_back(item);
            }
        }
        return result;
    }
};

struct ColorSpecification : Specification<Product> {
    Color color;

    explicit ColorSpecification(Color color)
        : color(color) {}

    bool is_satisfied(Product* item) const override {
        return item->color == color;
    }
};

struct SizeSpecification : Specification<Product> {
    Size size;

    explicit SizeSpecification(Size size)
        : size(size) {}

    bool is_satisfied(Product* item) const override {
        return item->size == size;
    }
};

int main() {
    Product apple{"Apple", Color::Green, Size::Small };
    Product tree {"Tree" , Color::Green, Size::Large };
    Product house{"House", Color::Blue , Size::Small};

    std::vector<Product*> items{&apple, &tree, &house};

    BetterFilter bf;
    auto green_and_small_items = bf.filter(items, ColorSpecification(Color::Green) && SizeSpecification(Size::Small));

    for (auto& item : green_and_small_items) {
        std::cout << item->name << " is \033[32mgreen\033[0m and small\n";
    }
    return 0;
}
