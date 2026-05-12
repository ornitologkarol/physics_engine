#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "entities.hpp"

class game {
    public:
    std::vector<std::unique_ptr<circle>> objects;
    sf::VertexArray triangles;
    unsigned int capacity = 10000;

    const int window_width;
    const int window_height;

    game(int width, int height);
    void update(float dt);
    void add(float n_radius, int n_sides, vector2d n_position, vector2d n_velocity, float n_mass);
    void draw(sf::RenderWindow &window);
};
