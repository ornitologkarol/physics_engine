#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include "basic_math.hpp"
#include "game.hpp"
#include "config.hpp"

int main() {
    sf::RenderWindow window;
    window.create(sf::VideoMode(Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT), "Engine");
    window.setFramerateLimit(60);
    sf::Event event;
    sf::Clock clock;
    float dt;

    game game(Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT);
    
    while(window.isOpen()) {
        std::cout << int(1.0/dt) << " " << game.objects.size() << "\n";

        sf::Time elapsed = clock.restart();
        dt = elapsed.asSeconds(); // calculating deltatime
        
        while(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed)
                window.close();
            if(event.type == sf::Event::MouseButtonPressed) {
                if(event.mouseButton.button == sf::Mouse::Left) {
                    game.add(100, 20, vector2d(event.mouseButton.x, event.mouseButton.y), vector2d(0, 45, false),20.0);
                }
            }
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) { // adding a circle
            for(int i = 0; i < 5; i++)
            game.add(20.0, 4, vector2d(800.0, 600.0), vector2d(500, 45, false),20.0);
        }

//        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            game.update(dt);
        game.draw(window);

    }
}
