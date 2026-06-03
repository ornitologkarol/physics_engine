#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include "basic_math.hpp"
#include "entities.hpp"

#define DOUBLE_PI 6.28318530718f

circle::circle(float n_radius, int n_sides, vector2d n_position, vector2d n_velocity, float n_mass) :
    is_static(false),
    radius(n_radius),
    sides(n_sides),
    position(n_position),
    velocity(n_velocity),
    force(vector2d()),
    mass(n_mass),
    angle(0.f),
    red(rand()%256),
    green(rand()%256),
    blue(rand()%256) {}

circle::circle(float n_radius, int n_sides, vector2d n_position) :
    is_static(true),
    radius(n_radius),
    sides(n_sides),
    position(n_position),
    velocity(),
    force(vector2d()),
    mass(0),
    angle(0.f),
    red(rand()%256),
    green(rand()%256),
    blue(rand()%256) {}
    
void fill(sf::VertexArray &triangles, int &idx, vector2d position, vector2d point_first, vector2d point_second, int red, int green, int blue) {
        triangles[idx].position.x = position.x;
        triangles[idx].position.y = position.y;
        triangles[idx].color = sf::Color(red,green,blue);
        idx++;
        triangles[idx].position.x = point_first.x;
        triangles[idx].position.y = point_first.y;
        triangles[idx].color = sf::Color(red, green, blue);
        idx++;
        triangles[idx].position.x = point_second.x;
        triangles[idx].position.y = point_second.y;
        triangles[idx].color = sf::Color(red, green, blue);
        idx++;
}

void circle::fill_array(sf::VertexArray &triangles, int &idx) {
    // in sf vertex array, each three points represent a single triangle
    // using radius i calulate every small triangle (that starts in the center) for each side of my shape 
    float angle_change = DOUBLE_PI / static_cast<float>(sides);
    float rotation = angle;
    
    vector2d point_first = vector2d(radius, rotation, true); 
    point_first.add(position); // vector = (point - center) + center = point
    vector2d point_second; 
    vector2d temp_point_first = point_first; // if i calulated new point_second for the last point (which is the same as the very first) there would be a slight gap, due to floating point errors
    
    rotation += angle_change;
    for (int i = 0; i < sides - 1; i++) {
        point_second = vector2d(radius, rotation, true);
        point_second.add(position);

        fill(triangles, idx, position, point_first, point_second, red, green, blue);

        point_first = point_second;
        rotation += angle_change;
    }
    fill(triangles, idx, position, point_first, temp_point_first, red, green, blue);
}

void circle::move(float dt) {
    position.x += velocity.x * dt;
    position.y += velocity.y * dt;
}

void circle::accelerate(float dt) {
    velocity.x += force.x/mass * dt;
    velocity.y += force.y/mass * dt;
}
