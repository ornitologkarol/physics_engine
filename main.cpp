#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include "basic_math.hpp"
#include "game.hpp"

const int window_width = 1600;
const int window_height = 1200;

int main() {
    sf::RenderWindow window;
    window.create(sf::VideoMode(window_width, window_height), "Engine");
    window.setFramerateLimit(60);
    sf::Event event;
    sf::Clock clock;
    float dt;

    game game(window_width, window_height);
    
    while(window.isOpen()) {
        std::cout << int(1.0/dt) << " " << game.objects.size() << "\n";

        sf::Time elapsed = clock.restart();
        dt = elapsed.asSeconds(); // calculating deltatime
        
        while(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed)
                window.close();
            if(event.type == sf::Event::MouseButtonPressed) {
                if(event.mouseButton.button == sf::Mouse::Left) {
                    game.add(100.0, 30, vector2d(event.mouseButton.x, event.mouseButton.y), vector2d(),20.0);
                }
            }
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) { // adding a circle
            game.add(10.0, 30, vector2d(800.0, 600.0), vector2d(500, 45, false),20.0);
        }

        game.update(dt);
        game.draw(window);

    }
}
