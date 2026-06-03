#pragma once

class vector2d {
    public:
    float x;
    float y;

    vector2d();
    vector2d(float a, float b);
    vector2d(float lenght, float angle, bool radians);
    float lenght();
    void add(vector2d v);
    void subtract(vector2d v);
    void multiply(float num);
    vector2d perpendicular();
    void rotate(float angle);
    void normalize();
};

float dot(vector2d a, vector2d b);
