#include <SFML/Graphics.hpp>
#include <vector>

#include "basic_math.hpp"
#include "entities.hpp"
#include "game.hpp"
#include "space.hpp"

game::game(int width, int height) : triangles(sf::PrimitiveType::Triangles, 10000) {}

void game::update(float dt) {
    for(auto &obj : objects) {
         obj.move(dt);
    }
   space_partition(objects);
}

void game::add(float n_radius, int n_sides, vector2d n_position, vector2d n_velocity, float n_mass) {
    objects.push_back(circle(n_radius, n_sides, n_position, n_velocity, n_mass));
}

void game::draw(sf::RenderWindow &window) {
    window.clear(sf::Color::Black);
    int idx = 0;
    for(auto &shape : objects) { // drawing all the circles
        if (capacity <= idx + shape.sides * 3) { // checking capacity
            capacity *= 2;
            triangles.resize(capacity);
        }
        shape.fill_array(triangles, idx);
    }
    window.draw(triangles);
    window.display();
}

