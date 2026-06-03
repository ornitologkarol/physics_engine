#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "entities.hpp"

class game {
    public:
    std::vector<circle> objects;
    sf::VertexArray triangles;
    unsigned int capacity = 10000;

    game(int width, int height);
    void update(float dt);
    void add(float n_radius, int n_sides, vector2d n_position, vector2d n_velocity, float n_mass);
    void add_static(float n_radius, int n_sides, vector2d n_position);
    void draw(sf::RenderWindow &window);
};
