#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

#include "basic_math.hpp"
#include "entities.hpp"
#include "collision.hpp"
#include "game.hpp"

void space_partition(std::vector<std::unique_ptr<circle>> &shapes, const int window_width, const int window_height) {
    // needs checking neighbours and edgecase handling
    const int rows = 5;
    const int cols = 5;

    static std::vector<int> space[rows][cols];
    for(int r=0; r < rows; r++) {
        for(int c=0; c < cols; c++) {
            space[r][c].clear();
        }
    }

    for(int i=0; i<shapes.size(); i++) {
        int r = shapes[i]->position.x / window_width * rows;
        int c = shapes[i]->position.y / window_height * cols;
        space[r][c].push_back(i);
    }

    for(int r=0; r <rows; r++) {
        for(int c=0; c <cols; c++) {
            for(int i=0; i<space[r][c].size(); i++) {
                for(int j=i+1; j < space[r][c].size(); j++) {
                    circle_collision(*shapes[space[r][c][i]], *shapes[space[r][c][j]]);
                }
            }
        }
    }
}


game::game(int width, int height) : window_width(width), window_height(height), triangles(sf::PrimitiveType::Triangles, 10000) {}

void game::update(float dt) {
    for(auto &obj : objects) {
         obj->move(dt);
         wall_collision(*obj, window_width, window_height);
    }
    space_partition(objects, window_width, window_height);
}

void game::add(float n_radius, int n_sides, vector2d n_position, vector2d n_velocity, float n_mass) {
    objects.push_back(std::make_unique<circle>(n_radius, n_sides, n_position, n_velocity, n_mass));
}

void game::draw(sf::RenderWindow &window) {
    window.clear(sf::Color::Black);
    int idx = 0;
    for(auto &shape : objects) { // drawing all the circles
        if (capacity <= idx + shape->sides * 3) { // checking capacity
            capacity *= 2;
            triangles.resize(capacity);
        }
        shape->fill_array(triangles, idx);
    }
    window.draw(triangles);
    window.display();
}
