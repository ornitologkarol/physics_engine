#include <SFML/Graphics.hpp>

const int window_width = 1600;
const int window_height = 1200;

int main() {
    sf::RenderWindow window;
    window.create(sf::VideoMode(window_width, window_height), "Engine");
    window.setFramerateLimit(60);
    sf::Event event;

    sf::VertexArray triangle(sf::PrimitiveType::Triangles, 3);
    
    triangle[0].position = sf::Vector2f(800.f, 600.f);
    triangle[1].position = sf::Vector2f(600.f, 800.f);
    triangle[2].position = sf::Vector2f(1000.f, 800.f);

    triangle[0].color = sf::Color::Red;
    triangle[1].color = sf::Color::Blue;
    triangle[2].color = sf::Color::Green;
    while(window.isOpen()) {
        while(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed)
                window.close();
        }
        window.clear(sf::Color::Black);
        window.draw(triangle);
        window.display();
    }
}
