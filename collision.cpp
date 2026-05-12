#include <cmath>
#include <vector>
#include "basic_math.hpp"
#include "entities.hpp"
#include "collision.hpp"

bool wall_collision(circle& shape, int window_width, int window_height) {
    vector2d mtv;

    // hardcodes collision resolvers should be temporary
    
    if (shape.position.x - shape.radius < 0) {
        mtv.x = shape.radius - shape.position.x;
        shape.velocity.x *= -1;}
    else if(shape.position.x + shape.radius > window_width){
        mtv.x = (window_width - shape.position.x) - shape.radius;
        shape.velocity.x *= -1;}
    if (shape.position.y - shape.radius < 0) {
        mtv.y = shape.radius - shape.position.y;
        shape.velocity.y *= -1;}
    else if(shape.position.y + shape.radius > window_height) {
        mtv.y = (window_height - shape.position.y) - shape.radius;
        shape.velocity.y *= -1;}

    shape.position.add(mtv);

    return (mtv.x != 0 || mtv.y != 0);
}

bool circle_collision(circle &shape1, circle &shape2) {
    // hardcoded collision resolver should be temporary
    float dist = sqrt((shape1.position.x - shape2.position.x) * (shape1.position.x - shape2.position.x)  + (shape1.position.y - shape2.position.y) *(shape1.position.y - shape2.position.y));
    float rad_sum = shape1.radius + shape2.radius;
    vector2d mtv;
    if (dist < rad_sum) {
        mtv = shape2.position;
        mtv.subtract(shape1.position);
        if (fabs(dist) <= 1e-9) { // risk of division by zero
            dist = 1;
            mtv = vector2d(1,0);
        }
        mtv.multiply((rad_sum - dist)/dist);
        mtv.multiply(1.0/2.0);
        shape2.position.add(mtv);
        shape1.position.subtract(mtv);
        return true;
    }
    return false;
}


void space_partition(std::vector<circle> &shapes) {
    const int rows = 5;
    const int cols = 5; 
}
