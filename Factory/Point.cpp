#include <cmath>
#include <gtest/gtest.h>


class Point;
class PointFactory;

class Point {
public:
    friend class OutsidePointFactory;
    float getX() const { return x; }
    float getY() const { return y; }

class InternalPointFactory {
public:
    static Point NewCartesian(float x, float y) {
        return Point(x, y);
    }
    static Point NewPolar(float radius, float angle) {
        return Point(radius * cos(angle), radius * sin(angle));
    }
};

private:
    float x, y;
    Point(const float x, const float y)
        : x(x), y(y) { }
};

class OutsidePointFactory {
public:
    static Point NewCartesian(float x, float y) {
        return Point(x, y);
    }
    static Point NewPolar(float radius, float angle) {
        return Point(radius * cos(angle), radius * sin(angle));
    }
};

class PointTest : public ::testing::Test {
protected:
    // Helper function to compare floating point values
    bool isEqual(float a, float b, float epsilon = 0.0001f) {
        return std::fabs(a - b) < epsilon;
    }
};

TEST_F(PointTest, OutsideFactoryCartesianPoint) {
    auto point = OutsidePointFactory::NewCartesian(3.0f, 4.0f);
    EXPECT_TRUE(isEqual(point.getX(), 3.0f));
    EXPECT_TRUE(isEqual(point.getY(), 4.0f));
}

TEST_F(PointTest, OutsideFactoryPolarPoint) {
    float radius = 5.0f;
    float angle = 0.927f;  // ~53.13 degrees
    auto point = OutsidePointFactory::NewPolar(radius, angle);

    float expectedX = radius * std::cos(angle);
    float expectedY = radius * std::sin(angle);

    EXPECT_TRUE(isEqual(point.getX(), expectedX));
    EXPECT_TRUE(isEqual(point.getY(), expectedY));
}

TEST_F(PointTest, InternalFactoryCartesianPoint) {
    auto point = Point::InternalPointFactory::NewCartesian(3.0f, 4.0f);
    EXPECT_TRUE(isEqual(point.getX(), 3.0f));
    EXPECT_TRUE(isEqual(point.getY(), 4.0f));
}

TEST_F(PointTest, InternalFactoryPolarPoint) {
    float radius = 5.0f;
    float angle = 0.927f;  // ~53.13 degrees
    auto point = Point::InternalPointFactory::NewPolar(radius, angle);

    float expectedX = radius * std::cos(angle);
    float expectedY = radius * std::sin(angle);

    EXPECT_TRUE(isEqual(point.getX(), expectedX));
    EXPECT_TRUE(isEqual(point.getY(), expectedY));
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
