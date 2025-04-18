#include <cmath>


class Point;
class PointFactory;

class Point {
public:
    friend class OutsidePointFactory;

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

int main() {
    auto p1 = OutsidePointFactory::NewCartesian(3.0f, 4.0f);
    auto p2 = Point::InternalPointFactory::NewPolar(5.0f, 0.927f);
}
