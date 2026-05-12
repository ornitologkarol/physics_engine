#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <iostream>
#include "basic_math.hpp"
#include "entities.hpp"
#include "collision.hpp"

const int window_width = 1600;
const int window_height = 1200;

int main() {
    sf::RenderWindow window;
    window.create(sf::VideoMode(window_width, window_height), "Engine");
    window.setFramerateLimit(60);
    sf::Event event;
    sf::Clock clock;
    float dt;
    
    int idx = 0;
    sf::VertexArray triangle(sf::PrimitiveType::Triangles, 10000);
    unsigned int capacity = 10000;
    std::vector<std::unique_ptr<circle>> shapes;
    
    while(window.isOpen()) {
        std::cout << int(1.0/dt) << " " << shapes.size() << "\n";

        sf::Time elapsed = clock.restart();
        dt = elapsed.asSeconds(); // calculating deltatime
        
        while(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed)
                window.close();
            if(event.type == sf::Event::MouseButtonPressed) {
                if(event.mouseButton.button == sf::Mouse::Left) {
                    shapes.push_back(std::make_unique<circle>(100.0, 30, vector2d(event.mouseButton.x, event.mouseButton.y), vector2d(),20.0));
                }
            }
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) { // adding a circle
            shapes.push_back(std::make_unique<circle>(10.0, 30, vector2d(800.0, 600.0), vector2d(500, 45, false),20.0));
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)) { // all movement here
            for(auto &shape : shapes) { // moving all the circles
                shape->move(dt);
                wall_collision(*shape, window_width, window_height);
            }
            for(int i=0; i<shapes.size(); i++) {
                for(int j=i; j < shapes.size(); j++) {
                    circle_collision(*shapes[i], *shapes[j]);
                }
            }
        }

        // drawing 
        window.clear(sf::Color::Black);
        idx = 0;
        for(auto &shape : shapes) { // drawing all the circles
            if (capacity <= idx + shape->sides * 3) { // checking capacity
                capacity *= 2;
                triangle.resize(capacity);
            }
            shape->fill_array(triangle, idx);
        }
        window.draw(triangle);
        window.display();
        // end of drawing
    }
}
