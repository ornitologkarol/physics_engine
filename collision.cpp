#include <cmath>
#include <cstdlib>
#include "basic_math.hpp"
#include "entities.hpp"
#include "collision.hpp"

bool wall_collision(circle& shape, int window_width, int window_height, float slop) {
    vector2d mtv;
    // hardcodes collision resolvers should be temporary

    float x = -1;
    
    if (shape.position.x - shape.radius + slop < 0) {
        mtv.x = shape.radius - shape.position.x;
        shape.velocity.x *= x;
    }
    else if(shape.position.x + shape.radius - slop > window_width){
        mtv.x = (window_width - shape.position.x) - shape.radius;
        shape.velocity.x *= x;
    }
    if (shape.position.y - shape.radius + slop < 0) {
        mtv.y = shape.radius - shape.position.y;
        shape.velocity.y *= x;
    }
    else if(shape.position.y + shape.radius - slop > window_height) {
        mtv.y = (window_height - shape.position.y) - shape.radius;
        shape.velocity.y *= x;
    }

    shape.position.add(mtv);
    
    return (mtv.x != 0 || mtv.y != 0);
    }

bool circle_collision(circle &shape1, circle &shape2, float slop) {
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
            float temp = ((double)rand()) / RAND_MAX;

            mtv = vector2d(temp,1-temp);
        }

        mtv.multiply((rad_sum - dist)/dist);

        //applying mtv to objs
        mtv.multiply(1.0/2.0);
        shape2.position.add(mtv);
        shape1.position.subtract(mtv);
        return true;
    }
    return false;
}

void collision_resolve(circle& shape1, circle& shape2) {
    // impulse based collision resolver
    // this works this way:
    // - we project relative speed onto the normal (collision axis) = vreln
    // - we use this equation for magnitude j = (1+e)*vreln / (1/m1 + 1/m2)
    // - we make it a vector J = j*n
    // - delta v1 = J/m1, delta v2 = -J/m2
    
    
    float e = 1.f;
    
    vector2d normal = shape1.position;
    normal.subtract(shape2.position);
    float distance = normal.lenght();
    normal.multiply(1/distance);

    vector2d v_rel = shape2.velocity;
    v_rel.subtract(shape1.velocity);

    float normal_rel = dot(normal, v_rel);
    float inv_mass = 1.f/shape1.mass + 1.f/shape2.mass;
    float magnitude = (1.f+e) * normal_rel / inv_mass;

    vector2d impulse = normal;
    impulse.multiply(magnitude);
    vector2d delta1 = impulse;
    delta1.multiply(1.f/shape1.mass);
    vector2d delta2 = impulse;
    delta2.multiply(1.f/shape2.mass);

    shape1.velocity.add(delta1);
    shape2.velocity.subtract(delta2);
}
