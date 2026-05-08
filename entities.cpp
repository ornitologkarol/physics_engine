#include <SFML/Graphics.hpp>
#include "basic_math.hpp"
#include "entities.hpp"


circle::circle(float n_radius, int n_sides, vector2d n_position, vector2d n_velocity, float n_mass) :
    radius(n_radius),
    sides(n_sides),
    position(n_position),
    velocity(n_velocity),
    force(vector2d()),
    mass(n_mass),
    angle(0.0) {}

void circle::fill_array(sf::VertexArray &triangles, int &idx) {
    // in sf vertex array each three points represent a single triangle
    // using radius i calulate every small triangle (that starts in the center) for each side of my circle 
    float angle_change = 6.28 / sides;
    float rotation = angle;
    
    vector2d point_first = vector2d(radius, rotation, true);
    point_first.add(position);
    
    vector2d point_second; 
    rotation += angle_change;

    vector2d temp_point_first = point_first;
    
    for (int i = 0; i < sides; i++) {
        if (i == sides - 1) {
            point_second = temp_point_first;
            // if i calulated new point_second for the last point (which is the same as the very first) there would be a slight gap, due to floating point errors
        } else {
            point_second = vector2d(radius, rotation, true);
            point_second.add(position);
        }

        triangles[idx].position.x = position.x;
        triangles[idx].position.y = position.y;
        triangles[idx].color = sf::Color::White;
        idx++;
        triangles[idx].position.x = point_first.x;
        triangles[idx].position.y = point_first.y;
        triangles[idx].color = sf::Color::White;
        idx++;
        triangles[idx].position.x = point_second.x;
        triangles[idx].position.y = point_second.y;
        triangles[idx].color = sf::Color::White;
        idx++;

        point_first = point_second;
        rotation += angle_change;
    }
}

void circle::move(float dt) {
    position.x += velocity.x * dt;
    position.y += velocity.y * dt;
}
