#include <SFML/Graphics.hpp>
#include "basic_math.hpp"
#include "entities.hpp"

const int window_width = 1600;
const int window_height = 1200;

int main() {
    sf::RenderWindow window;
    window.create(sf::VideoMode(window_width, window_height), "Engine");
    window.setFramerateLimit(60);
    sf::Event event;
    sf::Clock clock;

    float dt;
    sf::VertexArray triangle(sf::PrimitiveType::Triangles, 10000);
    int idx = 0;
    circle shape = circle(100.0, 30, vector2d(800.0, 600.0), vector2d(500, 45, false), 20.0);
    
    while(window.isOpen()) {

        sf::Time elapsed = clock.restart();
        dt = elapsed.asSeconds();
        
        while(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed)
                window.close();
        }

        shape.move(dt);

        window.clear(sf::Color::Black);
        idx = 0;
        shape.fill_array(triangle, idx);
        window.draw(triangle);
        window.display();
    }
}
