#include <cmath>
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

bool wall_collision(circle& shape, int window_width, int window_height, vector2d &mtv) {
    // hardcoded collision resolvers should be temporary
    bool result = false;
    
    if (shape.position.x - shape.radius < 0.f) {
        mtv.x = shape.radius - shape.position.x;
        shape.velocity.x *= -1;
        result = true;
    }
    else if(shape.position.x + shape.radius > static_cast<float>(window_width)){
        mtv.x = (static_cast<float>(window_width) - shape.position.x) - shape.radius;
        shape.velocity.x *= -1;
        result = true;
    }
    if (shape.position.y - shape.radius < 0.f) {
        mtv.y = shape.radius - shape.position.y;
        shape.velocity.y *= -1;
        result = true;
    }
    else if(shape.position.y + shape.radius > static_cast<float>(window_height)) {
        mtv.y = (static_cast<float>(window_height) - shape.position.y) - shape.radius;
        result = true;
    }

    return result;
}

bool circle_collision(circle &shape1, circle &shape2, vector2d &mtv) {
    // hardcoded collision resolver should be temporary
    float dist = (shape1.position.x - shape2.position.x) * (shape1.position.x - shape2.position.x)  + (shape1.position.y - shape2.position.y) *(shape1.position.y - shape2.position.y);
    float rad_sum = shape1.radius + shape2.radius;
    if (dist >= rad_sum * rad_sum )
        return false; // sqrt is expensive so this broadphase tries to skip it
    dist = std::sqrt(dist); 
    if (dist < rad_sum) {
        mtv = shape2.position;
        mtv.subtract(shape1.position);
        if (fabs(dist) <= 1e-9) { // risk of division by zero
            dist = 1;
            mtv = vector2d(1,0);
        }
        mtv.multiply((rad_sum - dist)/dist);
        return true;
    }
    return false;
}
