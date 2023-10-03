// https://youtu.be/icZj67PTFhc?si=tiSwjb48qH9QkfMU
#include <array>
#include <chrono>
#include <queue>
#include <SFML/Graphics.hpp>
#include <iostream>

// #include "randomStuff/DrawMap.cpp"
int main()
{
    sf::RenderWindow window(sf::VideoMode(1024,1024), "SFML works!");

    // Load texture
    sf::Texture texture;
    if(!texture.loadFromFile("randomStuff/tileSprite.png")) { std::cout << "file did not infact load" << std::endl; }
    texture.setSmooth(true);

    // Setting sprites
    sf::Sprite unvisted, visited, start, end, path;
    {
        unvisted.setTexture(texture);
        unvisted.setTextureRect(sf::IntRect(0, 0, 32, 32));
        // window.draw(unvisted);

        visited.setTexture(texture);
        visited.setTextureRect(sf::IntRect(32,0,32,32));

        start.setTexture(texture);
        start.setTextureRect(sf::IntRect(64,0,32,32));

        end.setTexture(texture);
        end.setTextureRect(sf::IntRect(96,0,32,32));

        path.setTexture(texture);
        path.setTextureRect(sf::IntRect(128,0,32,32));
    }
    // sf::CircleShape shape(100.f);
    // shape.setFillColor(sf::Color::Green);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(path);
        window.display();
    }

    return 0;
}
