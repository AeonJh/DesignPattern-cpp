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


#include <string>
#include <vector>
#include <gtest/gtest.h>


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

class ProductFilterTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create some test products
        apple = new Product{"Apple", Color::Green, Size::Small};
        tree = new Product{"Tree", Color::Green, Size::Large};
        house = new Product{"House", Color::Blue, Size::Large};
        car = new Product{"Car", Color::Red, Size::Medium};

        items = {apple, tree, house, car};
    }

    void TearDown() override {
        delete apple;
        delete tree;
        delete house;
        delete car;
    }

    Product* apple;
    Product* tree;
    Product* house;
    Product* car;
    std::vector<Product*> items;
};

TEST_F(ProductFilterTest, ColorSpecification) {
    ColorSpecification green(Color::Green);
    BetterFilter bf;

    auto green_items = bf.filter(items, green);

    ASSERT_EQ(green_items.size(), size_t(2));
    EXPECT_EQ(green_items[0]->name, "Apple");
    EXPECT_EQ(green_items[1]->name, "Tree");
}

TEST_F(ProductFilterTest, SizeSpecification) {
    SizeSpecification large(Size::Large);
    BetterFilter bf;

    auto large_items = bf.filter(items, large);

    ASSERT_EQ(large_items.size(), size_t(2));
    EXPECT_EQ(large_items[0]->name, "Tree");
    EXPECT_EQ(large_items[1]->name, "House");
}

TEST_F(ProductFilterTest, AndSpecification) {
    ColorSpecification green(Color::Green);
    SizeSpecification large(Size::Large);
    AndSpecification<Product> green_and_large(green, large);
    BetterFilter bf;

    auto green_large_items = bf.filter(items, green_and_large);

    ASSERT_EQ(green_large_items.size(), size_t(1));
    EXPECT_EQ(green_large_items[0]->name, "Tree");
}

TEST_F(ProductFilterTest, OperatorAndSpecification) {
    BetterFilter bf;
    auto green_large_items = bf.filter(items,
        ColorSpecification(Color::Green) && SizeSpecification(Size::Large));

    ASSERT_EQ(green_large_items.size(), size_t(1));
    EXPECT_EQ(green_large_items[0]->name, "Tree");
}

TEST_F(ProductFilterTest, EmptyResults) {
    ColorSpecification red(Color::Red);
    SizeSpecification small(Size::Small);
    AndSpecification<Product> red_and_small(red, small);
    BetterFilter bf;

    auto red_small_items = bf.filter(items, red_and_small);

    EXPECT_TRUE(red_small_items.empty());
}

TEST_F(ProductFilterTest, OlderFilterByColor) {
    OlderProductFilter of;
    auto green_items = of.filter_by_color(items, Color::Green);

    ASSERT_EQ(green_items.size(), size_t(2));
    EXPECT_EQ(green_items[0]->name, "Apple");
    EXPECT_EQ(green_items[1]->name, "Tree");
}

TEST_F(ProductFilterTest, OlderFilterBySize) {
    OlderProductFilter of;
    auto large_items = of.filter_by_size(items, Size::Large);

    ASSERT_EQ(large_items.size(), size_t(2));
    EXPECT_EQ(large_items[0]->name, "Tree");
    EXPECT_EQ(large_items[1]->name, "House");
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
