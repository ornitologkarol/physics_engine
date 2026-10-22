#include <cmath>
#include <algorithm>
#include <iostream>
#include <vector>

#include "entities.hpp"
#include "config.hpp"
#include "basic_math.hpp"
#include "collision.hpp"
#include "space.hpp"


void space_partition(std::vector<circle> &shapes) {
    constexpr int rows = Config::WINDOW_WIDTH / Config::MAX_RADIUS;
    constexpr int columns = Config::WINDOW_HEIGHT / Config::MAX_RADIUS;

    static int grid[rows * columns]; // our grid stores the index of the last obj each cell
    for(int i=0; i< rows * columns; i++) {
        grid[i] = -1;
    }
    
   static std::vector<int> prev_idx; // the prev_idx stores the index to the previous object in a cell for each obj, -1 i this is the first obj
   prev_idx.clear();

   static std::vector<int> big_idx; // for shapes with radius > max_radius we cannot use space partitioning 
   big_idx.clear();

    int idx = 0;
    for (auto &shape : shapes) {
        if(shape.radius > static_cast<float>(Config::MAX_RADIUS)) { // we check if the object isnt to big
            big_idx.push_back(idx);
            prev_idx.push_back(-1);
            idx++;
            continue;
        }
        int r = std::floor(shape.position.x * rows / Config::WINDOW_WIDTH);
        int c = std::floor(shape.position.y * columns / Config::WINDOW_HEIGHT);

        r = std::clamp(r, 0, rows - 1);
        c = std::clamp(c, 0, columns - 1);

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
                            vector2d mtv, contact;
                            if( m < l && final_collision(shapes[m], shapes[l], mtv, contact)) {
                                collision_resolve(shapes[m], shapes[l], mtv, contact);
                            }
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
             if (shapes[j].radius > Config::MAX_RADIUS) {
                 vector2d mtv, contact;
                 if (i < j && final_collision(shapes[i], shapes[j], mtv, contact)) {
                     collision_resolve(shapes[i], shapes[j], mtv, contact);
                 }
             } else {
                 vector2d mtv, contact;
                 if( i != j && final_collision(shapes[i], shapes[j], mtv, contact)) {
                     collision_resolve(shapes[i], shapes[j], mtv, contact);
                 }
             }
         }
     }
}
   
