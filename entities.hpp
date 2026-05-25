#pragma once

#include <SFML/Graphics.hpp>
#include "basic_math.hpp"

class circle {
    public:
    float radius;
    int sides; // circle has sides, because it is really a polygon and the larger the number of size, the closer it looks to a circle
    
    vector2d position;
    vector2d velocity;
    vector2d force;
    float mass;
    float angle;

    // color
    int red; 
    int green;
    int blue;

    circle(float n_radius, int n_sides, vector2d n_position, vector2d n_velocity, float n_mass);
    void fill_array(sf::VertexArray &triangles, int &idx);
    void move(float dt);
};
