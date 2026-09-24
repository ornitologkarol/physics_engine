#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <iostream>
#include "basic_math.hpp"
#include "game.hpp"
#include "config.hpp"

int main() {
    srand(time(NULL));
    sf::RenderWindow window;
    window.create(sf::VideoMode(Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT), "Engine");
    window.setFramerateLimit(60);
    window.setKeyRepeatEnabled(false);
    sf::Event event;
    sf::Clock clock;
    float dt;

    game game(Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT);
    
    while(window.isOpen()) {
//        std::cout << int(1.0/dt) << " " << game.objects.size() << "\n";

        sf::Time elapsed = clock.restart();
        dt = elapsed.asSeconds(); // calculating deltatime
        
        while(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed)
                window.close();
            if(event.type == sf::Event::MouseButtonPressed) {
                if(event.mouseButton.button == sf::Mouse::Left) {
                    game.add(100, 4, vector2d(event.mouseButton.x, event.mouseButton.y), vector2d(0, 0, false),20.0, 0.0);
                    game.objects[game.objects.size()-1].angle += 3.14/4.0;
                }
                if(event.mouseButton.button == sf::Mouse::Right) {
                    //game.add_static(100, 4, vector2d(event.mouseButton.x, event.mouseButton.y));
                    //game.objects[game.objects.size()-1].angle += 3.14/4.0;
                    game.add(10, 50, vector2d(event.mouseButton.x, event.mouseButton.y), vector2d(1000, 0, false),20.0, 0.0);
                }
            }
            if (event.type == sf::Event::KeyPressed) {
                if(event.key.code == sf::Keyboard::W) {
                    game.update(dt);
                }
            }
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) { // adding a circle
            for(int i=0; i<7; i++)
            game.add(10.0, 40, vector2d(10.0, 10.0), vector2d(1000, -45, false),20.0, 1.0);
        }

        game.update(dt);
        game.draw(window);

    }
}
