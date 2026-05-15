#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <iostream>

#include "basic_math.hpp"
#include "entities.hpp"
#include "collision.hpp"
#include "game.hpp"

void space_partition(std::vector<circle> &shapes, const int window_width, const int window_height) {
    // this was a bootleneck, make it work!
    const int rows = 30;
    const int columns = 16;
    
    int a = floor((float)window_height / columns / 2);
    int b = floor((float)window_width / rows / 2);
    const int max_radius = (a < b) ? a : b;

    static int grid[rows * columns];
    for(int i=0; i< rows * columns; i++) {
        grid[i] = -1;
    }
    
    std::vector<int> prev_idx;
    prev_idx.clear();

    int idx = 0;
    for (auto &shape : shapes) {
        int r = floor(shape.position.x * rows / window_width);
        int c = floor(shape.position.y * columns / window_height);

        if(r*columns + c >= rows*columns) {
            std::cout << "tutaj4 " << r << " "<< c << "\n";
        }

        if(grid[r * columns + c] == -1) { // this if is redundant, but it makes clear what the code does
            prev_idx.push_back(-1);
            grid[r * columns + c] = idx;
        } else {
            prev_idx.push_back(grid[r * columns + c]);
            grid[r * columns + c] = idx;
        }
        
        idx++;
    }

    static int qwerty[] = {-1, 0, 1};
    for(int i=0; i<rows*columns; i++) {
        int l = grid[i];
        while(l != -1) {
            int m = prev_idx[l];
            while(m != -1) {
                circle_collision(shapes[l], shapes[m]);
                m = prev_idx[m];
            }
            l = prev_idx[l];
        }
    }
    
}


game::game(int width, int height) : window_width(width), window_height(height), triangles(sf::PrimitiveType::Triangles, 10000) {}

void game::update(float dt) {
    for(auto &obj : objects) {
         obj.move(dt);
         wall_collision(obj, window_width, window_height);
    }
    space_partition(objects, window_width, window_height);
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
