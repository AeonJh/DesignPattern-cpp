/**
 * @file Rectangle.cpp
 * 
 * @brief Example of Liskov Substitution Principle violation
 * 
 * @details This code demonstrates a violation of the Liskov Substitution Principle (LSP)
 * by using a derived class (Square) that does not behave as expected when used in place of its base class (Rectangle).
 * The derived class modifies the behavior of the base class, leading to unexpected results.
 *
 * @Liskov Substitution Principle: If S is a declared subtype of T, objects of type S should 
 * behave as objects of type T are expected to behave, if they are treated as objects of type T.
 *
 * @author AeonJh
 * @date 2025-04-09
 */

#include <sstream>
#include <gtest/gtest.h>


class Rectangle {
protected:
    int width, height;
public:
    Rectangle(const int width, const int height)
        : width(width), height(height) { };
    virtual ~Rectangle() = default;

    int get_width() const { return width; }
    virtual void set_width(const int width) { this->width = width; }
    int get_height() const { return height; }
    virtual void set_height(const int height) { this->height = height; }

    int area() const { return width * height; }
};

class Square : public Rectangle {
public:
    Square(const int size)
        : Rectangle(size, size) { };

    void set_width(const int width) override {
        this->width = height = width;
    }
    void set_height(const int height) override {
        this->height = width = height;
    }
};

struct RectangleFactory {
    static Rectangle* create_rectangle(const int width, const int height);
    static Rectangle* create_square(const int size);

    static bool is_square(const Rectangle* r) {
        return r->get_width() == r->get_height();
    }
};

Rectangle* RectangleFactory::create_rectangle(const int width, const int height) {
    return new Rectangle(width, height);
}

Rectangle* RectangleFactory::create_square(const int size) {
    return new Square(size);
}

void process(Rectangle& r) {
    int w = r.get_width();
    r.set_height(10);

    std::cout << "Expected area: " << (w * 10)
            << ", got: " << r.area() << std::endl;
}

class RectangleTest : public ::testing::Test {
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

TEST_F(RectangleTest, RectangleArea) {
    Rectangle r(5, 4);
    EXPECT_EQ(r.area(), 20);
}

TEST_F(RectangleTest, RectangleDimensions) {
    Rectangle r(5, 4);
    EXPECT_EQ(r.get_width(), 5);
    EXPECT_EQ(r.get_height(), 4);
}

TEST_F(RectangleTest, RectangleSetDimensions) {
    Rectangle r(5, 4);
    r.set_width(3);
    r.set_height(6);
    EXPECT_EQ(r.get_width(), 3);
    EXPECT_EQ(r.get_height(), 6);
    EXPECT_EQ(r.area(), 18);
}

TEST_F(RectangleTest, SquareArea) {
    Square s(5);
    EXPECT_EQ(s.area(), 25);
}

TEST_F(RectangleTest, SquareDimensions) {
    Square s(5);
    EXPECT_EQ(s.get_width(), 5);
    EXPECT_EQ(s.get_height(), 5);
}

TEST_F(RectangleTest, LSPViolationWithSquare) {
    Square s(5);
    s.set_width(4);  // Should set both width and height to 4
    EXPECT_EQ(s.get_width(), 4);
    EXPECT_EQ(s.get_height(), 4);
    EXPECT_EQ(s.area(), 16);
}

TEST_F(RectangleTest, ProcessFunction) {
    Rectangle r(5, 5);
    process(r);
    EXPECT_EQ(output.str(), "Expected area: 50, got: 50\n");
}

TEST_F(RectangleTest, ProcessFunctionLSPViolation) {
    Square s(5);
    process(s);
    // The expected area should be 50 (5 * 10), but due to LSP violation,
    // both width and height become 10, resulting in area 100
    EXPECT_EQ(output.str(), "Expected area: 50, got: 100\n");
}

TEST_F(RectangleTest, RectangleFactoryCreation) {
    Rectangle* r = RectangleFactory::create_rectangle(4, 5);
    EXPECT_EQ(r->get_width(), 4);
    EXPECT_EQ(r->get_height(), 5);
    EXPECT_EQ(r->area(), 20);
    delete r;
}

TEST_F(RectangleTest, SquareFactoryCreation) {
    Rectangle* s = RectangleFactory::create_square(5);
    EXPECT_TRUE(RectangleFactory::is_square(s));
    EXPECT_EQ(s->get_width(), 5);
    EXPECT_EQ(s->get_height(), 5);
    EXPECT_EQ(s->area(), 25);
    delete s;
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
