#include <cmath>
#include "basic_math.hpp"

#define PI 3.14159265359f

vector2d::vector2d() : x(0), y(0) {}

vector2d::vector2d(float a, float b) : x(a), y(b) {}

vector2d::vector2d(float lenght, float angle, bool radians) {
    float rad = angle;
    if (!radians)
        rad *= PI / 180.f;
    x = std::cos(rad) * lenght;
    y = -std::sin(rad) * lenght; // negative sign, since the SFML has downward-pointing y-axis  
}

float vector2d::lenght() {
    return std::sqrt((x * x) + (y * y));
}

void vector2d::add(vector2d v) {
    x += v.x;
    y += v.y;
}

void vector2d::subtract(vector2d v) {
    x -= v.x;
    y -= v.y;
}

void vector2d::multiply(float num) {
    x *= num;
    y *= num;
}

vector2d vector2d::perpendicular() {
    return vector2d(-y, x);
}

void vector2d::rotate(float angle) {
    float cos_temp = std::cos(angle);
    float sin_temp = std::sin(angle);
    float x_temp = x;
    x = x * cos_temp - y * sin_temp;
    y = x_temp * sin_temp + y * cos_temp;
}

void vector2d::normalize() {
    float dist = lenght();
    if (dist != 0) {
        x /= dist;
        y /= dist;
    }
}

float dot(vector2d a, vector2d b) {
    return (a.x * b.x) + (a.y * b.y);
}
