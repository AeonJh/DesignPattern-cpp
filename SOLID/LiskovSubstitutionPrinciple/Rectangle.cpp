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


#include <iostream>


class Rectangle {
protected:
    int width, height;
public:
    Rectangle(const int width, const int height)
        : width(width), height(height) { };

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

int main() {
    Rectangle r(5, 5);
    process(r);
    Square s(5);
    process(s);

    Rectangle* r2 = RectangleFactory::create_rectangle(5, 5);
    process(*r2);
    Rectangle* s2 = RectangleFactory::create_square(5);
    if (RectangleFactory::is_square(s2)) {
        std::cout << "s2 is a square, do not process" << std::endl;
    } else {
        std::cout << "s2 is not a square, process" << std::endl;
        process(*s2);
    }

    return 0;
}
