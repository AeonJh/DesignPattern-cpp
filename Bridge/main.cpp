#include <memory>
#include <string>
#include <iostream>
#include <gtest/gtest.h>

// Implementation hierarchy - represents different rendering systems
struct Renderer {
    virtual void render_circle(float x, float y, float radius) = 0;
    virtual void render_rectangle(float x, float y, float width, float height) = 0;
    virtual ~Renderer() = default;
};

struct VectorRenderer : public Renderer {
    void render_circle(float x, float y, float radius) override {
        std::cout << "Drawing a circle as lines at (" << x << "," << y << ") with radius " << radius << std::endl;
    }
    
    void render_rectangle(float x, float y, float width, float height) override {
        std::cout << "Drawing a rectangle as lines at (" << x << "," << y << ") with width " << width << " and height " << height << std::endl;
    }
};

struct RasterRenderer : public Renderer {
    void render_circle(float x, float y, float radius) override {
        std::cout << "Drawing a circle as pixels at (" << x << "," << y << ") with radius " << radius << std::endl;
    }
    
    void render_rectangle(float x, float y, float width, float height) override {
        std::cout << "Drawing a rectangle as pixels at (" << x << "," << y << ") with width " << width << " and height " << height << std::endl;
    }
};

// Abstraction hierarchy - represents different shapes
struct Shape {
    Shape(std::shared_ptr<Renderer> renderer) : renderer_(renderer) {}
    virtual void draw() = 0;
    virtual void resize(float factor) = 0;
    virtual ~Shape() = default;

protected:
    std::shared_ptr<Renderer> renderer_;
};

struct Circle : public Shape {
    Circle(std::shared_ptr<Renderer> renderer, float x, float y, float radius)
        : Shape(renderer), x_(x), y_(y), radius_(radius) {}
    
    void draw() override {
        renderer_->render_circle(x_, y_, radius_);
    }
    
    void resize(float factor) override {
        radius_ *= factor;
    }
    
    float get_radius() const { return radius_; }

private:
    float x_, y_, radius_;
};

struct Rectangle : public Shape {
    Rectangle(std::shared_ptr<Renderer> renderer, float x, float y, float width, float height)
        : Shape(renderer), x_(x), y_(y), width_(width), height_(height) {}
    
    void draw() override {
        renderer_->render_rectangle(x_, y_, width_, height_);
    }
    
    void resize(float factor) override {
        width_ *= factor;
        height_ *= factor;
    }
    
    float get_width() const { return width_; }
    float get_height() const { return height_; }

private:
    float x_, y_, width_, height_;
};

// Unit tests
TEST(BridgePatternTest, CircleWithVectorRenderer) {
    auto vector_renderer = std::make_shared<VectorRenderer>();
    Circle circle(vector_renderer, 5.0f, 5.0f, 3.0f);
    
    testing::internal::CaptureStdout();
    circle.draw();
    std::string output = testing::internal::GetCapturedStdout();
    
    EXPECT_NE(output.find("Drawing a circle as lines"), std::string::npos);
    EXPECT_NE(output.find("(5,5)"), std::string::npos);
    EXPECT_NE(output.find("radius 3"), std::string::npos);
}

TEST(BridgePatternTest, CircleWithRasterRenderer) {
    auto raster_renderer = std::make_shared<RasterRenderer>();
    Circle circle(raster_renderer, 2.0f, 3.0f, 1.5f);
    
    testing::internal::CaptureStdout();
    circle.draw();
    std::string output = testing::internal::GetCapturedStdout();
    
    EXPECT_NE(output.find("Drawing a circle as pixels"), std::string::npos);
    EXPECT_NE(output.find("(2,3)"), std::string::npos);
    EXPECT_NE(output.find("radius 1.5"), std::string::npos);
}

TEST(BridgePatternTest, RectangleWithVectorRenderer) {
    auto vector_renderer = std::make_shared<VectorRenderer>();
    Rectangle rect(vector_renderer, 0.0f, 0.0f, 10.0f, 5.0f);
    
    testing::internal::CaptureStdout();
    rect.draw();
    std::string output = testing::internal::GetCapturedStdout();
    
    EXPECT_NE(output.find("Drawing a rectangle as lines"), std::string::npos);
    EXPECT_NE(output.find("(0,0)"), std::string::npos);
    EXPECT_NE(output.find("width 10"), std::string::npos);
    EXPECT_NE(output.find("height 5"), std::string::npos);
}

TEST(BridgePatternTest, RectangleWithRasterRenderer) {
    auto raster_renderer = std::make_shared<RasterRenderer>();
    Rectangle rect(raster_renderer, 1.0f, 2.0f, 8.0f, 4.0f);
    
    testing::internal::CaptureStdout();
    rect.draw();
    std::string output = testing::internal::GetCapturedStdout();
    
    EXPECT_NE(output.find("Drawing a rectangle as pixels"), std::string::npos);
    EXPECT_NE(output.find("(1,2)"), std::string::npos);
    EXPECT_NE(output.find("width 8"), std::string::npos);
    EXPECT_NE(output.find("height 4"), std::string::npos);
}

TEST(BridgePatternTest, CircleResize) {
    auto vector_renderer = std::make_shared<VectorRenderer>();
    Circle circle(vector_renderer, 0.0f, 0.0f, 2.0f);
    
    EXPECT_EQ(circle.get_radius(), 2.0f);
    circle.resize(1.5f);
    EXPECT_EQ(circle.get_radius(), 3.0f);
}

TEST(BridgePatternTest, RectangleResize) {
    auto raster_renderer = std::make_shared<RasterRenderer>();
    Rectangle rect(raster_renderer, 0.0f, 0.0f, 4.0f, 6.0f);
    
    EXPECT_EQ(rect.get_width(), 4.0f);
    EXPECT_EQ(rect.get_height(), 6.0f);
    rect.resize(0.5f);
    EXPECT_EQ(rect.get_width(), 2.0f);
    EXPECT_EQ(rect.get_height(), 3.0f);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}