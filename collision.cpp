#include <cmath>
#include "basic_math.hpp"
#include "entities.hpp"
#include "collision.hpp"
#include "clipping.hpp"

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

bool sat(circle &shape1, circle &shape2, vector2d &mtv, float slop) {
    // we always add mtv to shape2 and substract from shape 1
    // inverse is meant to account for that distinction
    // so mtv always points from B to A
    bool inverse = false;
    vector2d points1[shape1.sides], points2[shape2.sides], edges_perp1[shape1.sides], edges_perp2[shape2.sides];

    shape1.get_points_edges(points1, edges_perp1);
    shape2.get_points_edges(points2, edges_perp2);

    float min_magnitude = 10000000000;
    vector2d min_mtv;
    for (auto edge : edges_perp1) {
        edge.normalize();
        float min1, min2, max1, max2;
        
        min1 = dot(edge, points1[0]);
        max1 = min1;
        for(int i=1; i < shape1.sides; i++) {
            float projection = dot(edge, points1[i]);
            min1 = (projection < min1) ? projection : min1;
            max1 = (projection > max1) ? projection : max1;
        }

        min2 = dot(edge, points2[0]);
        max2 = min2;
        for(int i=1; i < shape2.sides; i++) {
            float projection = dot(edge, points2[i]);
            min2 = (projection < min2) ? projection : min2;
            max2 = (projection > max2) ? projection : max2;
        }

        if(!(min2 < max1 && max2 > min1)) return false;
        if(max1 - min2 < min_magnitude) {
           min_mtv = edge;
           min_magnitude = max1 - min2;
           inverse = false;
        }
        if(max2 - min1 < min_magnitude) {
            min_mtv = edge;
            min_magnitude = max2 - min1;
            inverse = true;
        }
    }

    for (auto edge : edges_perp2) {
        edge.normalize();
        float min1, min2, max1, max2;
        
        min1 = dot(edge, points1[0]);
        max1 = min1;
        for(int i=1; i < shape1.sides; i++) {
            float projection = dot(edge, points1[i]);
            min1 = (projection < min1) ? projection : min1;
            max1 = (projection > max1) ? projection : max1;
        }

        min2 = dot(edge, points2[0]);
        max2 = min2;
        for(int i=1; i < shape2.sides; i++) {
            float projection = dot(edge, points2[i]);
            min2 = (projection < min2) ? projection : min2;
            max2 = (projection > max2) ? projection : max2;
        }

        if(!(min2 < max1 && max2 > min1)) return false;
        if(max1 - min2 < min_magnitude) {
           min_mtv = edge;
           min_magnitude = max1 - min2;
           inverse = false;
        }
        if(max2 - min1 < min_magnitude) {
            min_mtv = edge;
            min_magnitude = max2 - min1;
            inverse = true;
        }
    }

    float dist = min_mtv.lenght();
    if (fabs(dist) <= 1e-9) { // risk of division by zero
        dist = 1;
        float temp = ((double)rand()) / RAND_MAX;

        min_mtv = vector2d(temp,1-temp);
    }
    min_mtv.multiply(min_magnitude/dist);
    if (inverse) min_mtv.multiply(-1);

    mtv = min_mtv;
    return true;
}

bool circle_collision(circle &shape1, circle &shape2, vector2d &mtv, float slop) {
 // hardcoded collision resolver should be temporary

    float dist = (shape1.position.x - shape2.position.x) * (shape1.position.x - shape2.position.x)  + (shape1.position.y - shape2.position.y) *(shape1.position.y - shape2.position.y);
    float rad_sum = shape1.radius + shape2.radius;
    if (dist >= rad_sum * rad_sum )
        return false;
    dist = sqrt(dist);
    if(dist + slop >= rad_sum) {
        return false;
    }
    vector2d min_mtv;
    if (dist < rad_sum) {
        min_mtv = shape2.position;
        min_mtv.subtract(shape1.position);
        if (fabs(dist) <= 1e-9) { // risk of division by zero
            dist = 1;
            float temp = ((double)rand()) / RAND_MAX;

            min_mtv = vector2d(temp,1-temp);
        }

        min_mtv.multiply((rad_sum - dist)/dist);

        mtv = min_mtv;
        return true;
    }
    return false;
}

void collision_resolve(circle& shape1, circle& shape2, vector2d mtv, vector2d contact) {
    // impulse based collision resolver
    // this works this way:
    // - we project relative speed onto the normal (collision axis) = vreln
    // - we use this equation for magnitude = (1+e)*vreln / { (1/m1 + 1/m2) + [((ra x n) x rb)/Ia + ((rb x n) x rb)/Ib] * n}
    // - in our case for 2d it reduces to: magnitude = (1*e) * vreln / { 1/m1 + 1/m2 + (ra x n)^2/Ia + (ra x n)^2/Ib }
    // - we make it an vector J = magnitude * normal
    // - delta v1 = J/m1, delta v2 = -J/m2
    
    
    float e = (shape1.e + shape2.e)/2;
    
    vector2d normal = mtv;
    normal.normalize();
    vector2d v_rel = shape2.velocity - shape1.velocity;
    float normal_rel = dot(normal, v_rel);
    float inv_mass = 1.f/shape1.mass + 1.f/shape2.mass;
    vector2d r1 = contact - shape1.position;
    vector2d r2 = contact - shape2.position;
    float i1 = shape1.polygon_inertia();
    float i2 = shape2.polygon_inertia();
    float part1 = cross(r1, normal) * cross(r1, normal) / i1;
    float part2 = cross(r2, normal) * cross(r2, normal) / i2;

    float magnitude = (1.f + e) * normal_rel / (inv_mass + part1 + part2);
    vector2d impulse = magnitude * normal;

    vector2d delta1 = impulse * (1.f/shape1.mass);
    vector2d delta2 = impulse * (1.f/shape2.mass);
    shape1.velocity.add(delta1);
    shape2.velocity.subtract(delta2);
    float delta_ang1 = cross(r1, impulse) / i1;
    float delta_ang2 = cross(r2, impulse) / i2;
    shape1.angle_velocity += delta_ang1;
    shape2.angle_velocity += delta_ang2;
}

bool final_collision(circle &shape1, circle &shape2, vector2d &res, vector2d &contact, float slop) {
    // broadest phase
    float dist = (shape1.position.x - shape2.position.x) * (shape1.position.x - shape2.position.x)  + (shape1.position.y - shape2.position.y) *(shape1.position.y - shape2.position.y);
    float rad_sum = shape1.radius + shape2.radius;
    if (dist >= rad_sum * rad_sum )
        return false;
    dist = sqrt(dist);
    if(dist + slop >= rad_sum) {
        return false;
    }

    vector2d mtv;

    // if not a circle, perform sat check
    if(shape1.sides < 30 || shape2.sides < 30) {
        if(!sat(shape1, shape2, mtv, slop)) return false;
    } else {
        if(!circle_collision(shape1, shape2, mtv, slop)) return false;
    }

    contact = contact_point(shape1, shape2, mtv);

    vector2d deltaA = mtv;
    vector2d deltaB = mtv;
    deltaA.multiply(shape2.mass/(shape1.mass + shape2.mass));
    deltaB.multiply(shape1.mass/(shape1.mass + shape2.mass));
    shape2.position.add(deltaB);
    shape1.position.subtract(deltaA);
    res = mtv;
    return true;
}
