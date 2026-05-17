#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>

#include "basic_math.hpp"
#include "entities.hpp"
#include "collision.hpp"
#include "game.hpp"

void space_partition(std::vector<circle> &shapes, const int window_width, const int window_height) {
    const int rows = window_width / 20;
    const int columns = window_height / 20;
    
    int a = floor((float)window_height / columns / 2);
    int b = floor((float)window_width / rows / 2);
    const int max_radius = (a < b) ? a : b;

    int grid[rows * columns]; // our grid stores the index of the last obj each cell
    for(int i=0; i< rows * columns; i++) {
        grid[i] = -1;
    }
    
   static std::vector<int> prev_idx; // the prev_idx stores the index to the previous object in a cell for each obj, -1 i this is the first obj
   prev_idx.clear();

   static std::vector<int> big_idx; // for shapes with radius > max_radius we cannot use space partitioning 
   big_idx.clear();

    int idx = 0;
    for (auto &shape : shapes) {
        if(shape.radius > max_radius) { // we check if the object isnt to big
            big_idx.push_back(idx);
            prev_idx.push_back(-1);
            idx++;
            continue;
        }
        int r = floor(shape.position.x * rows / window_width);
        int c = floor(shape.position.y * columns / window_height);

        if(grid[r * columns + c] == -1) { // this if is redundant, but it makes clear what the code does
            prev_idx.push_back(-1);
            grid[r * columns + c] = idx;
        } else {
            prev_idx.push_back(grid[r * columns + c]);
            grid[r * columns + c] = idx;
        }
        
        idx++;
    }

    for(int r=0; r < rows; r++) {
        for(int c=0; c < columns; c++) {
            int l = grid[r * columns + c];
            while(l != - 1) {
                for(int x = -1; x <= 1; x++) { // we need to check all the neighbouring cells also
                    for(int y=-1; y <= 1; y++) {
                        if(r+x >= rows || c+y >= columns || r+x <0 || c+y < 0) continue;
                        int m = grid[(r+x) * columns + (c+y)];
                        while (m != -1) {
                            if( m != l) circle_collision(shapes[l], shapes[m]);
                            m = prev_idx[m];
                        }
                    }
                }
                l = prev_idx[l];
            }
        }
    }

     for(int i : big_idx) { // if there are few big shapes this is linear
         for(int j=0; j<shapes.size(); j++) {
             if (i != j) circle_collision(shapes[i], shapes[j]);
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
