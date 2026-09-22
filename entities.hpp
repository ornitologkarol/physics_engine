#pragma once

#include <SFML/Graphics.hpp>
#include "basic_math.hpp"

class circle {
    public:
    bool is_static;
    float radius;
    int sides; // circle has sides, because it is really a polygon and the larger the number of size, the closer it looks to a circle
    
    vector2d position;
    vector2d velocity;
    vector2d force;
    float mass;
    float angle_velocity;
    float angle;
    float e; //number bettween 0 and 1 representing bounciness

    // color
    int red; 
    int green;
    int blue;

    circle(float n_radius, int n_sides, vector2d n_position, vector2d n_velocity, float n_mass, float n_e);
    circle(float n_radius, int n_sides, vector2d n_position); //for static objs
    void fill_array(sf::VertexArray &triangles, int &idx);
    void move(float dt);
    void angle_move(float dt);
    void accelerate(float dt);
    void get_points_edges(vector2d* points, vector2d* edges);
    float polygon_inertia();
    vector2d offset_vector(vector2d contact);
};
