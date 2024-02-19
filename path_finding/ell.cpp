#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <stdint.h>

class point
{
    public:
    uint8_t r = 0, b = 0, g = 0, a = 0;
    std::pair<uint16_t, uint16_t> cord;
    std::vector<uint16_t> neighbourIndex;


};

int main()
{
    sf::RenderWindow window(sf::VideoMode(1024,1024), "SFML works!");

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.display();
        window.clear();

    }

    return 0;
}
