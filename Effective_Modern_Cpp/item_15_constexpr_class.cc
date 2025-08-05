#include <iostream>
#include <string>
#include <vector>
// #include <boost/format.hpp>

using namespace std;

class Point {
public:
    constexpr Point(double _x = 0.0, double _y = 0.0) noexcept
                : x(_x), y(_y) 
    {}

    constexpr double getX() const noexcept
    { return x; }
    constexpr double getY() const noexcept
    { return y; }

    // only valid above C++ 14
    constexpr void setX(double _x) noexcept
    { x = _x; }
    constexpr void setY(double _y) noexcept
    { y = _y; }

private:
    double x{0.0}, y{0.0};
};

int main() {
    constexpr Point p1(3, 4);

    return 0;
}

