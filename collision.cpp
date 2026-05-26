#include <cmath>
#include <iostream>
#include "basic_math.hpp"
#include "entities.hpp"
#include "collision.hpp"

bool wall_broad(circle &shape, int window_width, int window_height) {
    return shape.position.x - shape.radius < 0.f || shape.position.x + shape.radius > window_width ||
           shape.position.y - shape.radius < 0.f || shape.position.y + shape.radius > window_height;
}
    
bool circle_broad(circle &shape1, circle &shape2) {
    float dist = (shape1.position.x - shape2.position.x) * (shape1.position.x - shape2.position.x)  + (shape1.position.y - shape2.position.y) *(shape1.position.y - shape2.position.y);
    float rad_sum = shape1.radius + shape2.radius;
    return dist >= rad_sum;
}

bool wall_collision(circle& shape, int window_width, int window_height, float slop, float percentage) {
    vector2d mtv;
    // hardcodes collision resolvers should be temporary
    
    if (shape.position.x - shape.radius + slop < 0) {
        mtv.x = shape.radius - shape.position.x;
        shape.velocity.x *= -1;}
    else if(shape.position.x + shape.radius - slop > window_width){
        mtv.x = (window_width - shape.position.x) - shape.radius;
        shape.velocity.x *= -1;}
    if (shape.position.y - shape.radius + slop < 0) {
        mtv.y = shape.radius - shape.position.y;
        shape.velocity.y *= -1;}
    else if(shape.position.y + shape.radius - slop > window_height) {
        mtv.y = (window_height - shape.position.y) - shape.radius;
        shape.velocity.y *= -1;}

    mtv.multiply(percentage);
    shape.position.add(mtv);
    
    return (mtv.x != 0 || mtv.y != 0);
    }

bool circle_collision(circle &shape1, circle &shape2, float slop, float percentage) {
 // hardcoded collision resolver should be temporary

    float dist = (shape1.position.x - shape2.position.x) * (shape1.position.x - shape2.position.x)  + (shape1.position.y - shape2.position.y) *(shape1.position.y - shape2.position.y);
    float rad_sum = shape1.radius + shape2.radius;
    if (dist >= rad_sum * rad_sum )
        return false;
    dist = sqrt(dist);
    if(dist + slop >= rad_sum) {
        return false;
    }
    vector2d mtv;
    if (dist < rad_sum) {
        mtv = shape2.position;
        mtv.subtract(shape1.position);
        if (fabs(dist) <= 1e-9) { // risk of division by zero
            dist = 1;

            mtv = vector2d(1,0);
        }

        mtv.multiply(percentage*(rad_sum - dist)/dist);

        //applying mtv to objs
        mtv.multiply(1.0/2.0);
        shape2.position.add(mtv);
        shape1.position.subtract(mtv);
        return true;
    }
    return false;}
