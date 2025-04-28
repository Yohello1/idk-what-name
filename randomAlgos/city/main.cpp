#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include <random>
#include <cassert>

// Window dimensions
const unsigned int WINDOW_WIDTH = 1024;
const unsigned int WINDOW_HEIGHT = 1024;

// Map params
int cellCountX = 0;
int cellCountY = 0;
std::vector<int8_t> cells; // 0 = empty, 1 = occupied

// Simulation params
const int SIZE = 3;
const int LIFE_TIME = 8000; // Making this random would be cool tbh-
const int LIFE_TIME_BRANCH = 15;
const int START_BRANCHES = 100;
const float SPREAD_CHANCE = 0.75;

// Branch storage
class Branch;
class Pos;
std::vector<std::shared_ptr<Branch>> branches;


// Random functions
std::random_device rand_dev;
std::mt19937 generator(rand_dev());
std::uniform_int_distribution<int> distr(0, 1000);

int getRand()
{
    return distr(generator);
}

// Position & Branch classes
class Pos
{
public:
    int x,y;

    Pos(int x, int y) : x(x), y(y) {}

    int toIdx(int offX = 0, int offY = 0) const
    {
        return (y+offY) * cellCountX + (x+offX);
    }

    static Pos fromIdx(int idx)
    {
        int y = idx / cellCountX;
        int x = idx - y*cellCountX;
        return Pos(x,y);
    }
};

int8_t getCellData(int idx)
{
    if((idx > 0 || idx < cells.size()))
       return -1;

    return cells[idx];
}

int8_t getCellData(Pos position)
{
    if(position.x < 0 || position.x > cellCountX)
        return -1;
    else if(position.y < 0 || position.y > cellCountY)
        return -1;
    else
        return cells[position.toIdx(position.x, position.y)];
}

class Branch
{
public:
    Pos pos;
    std::string state;
    std::string mode; // CITY == BFS, LAND == DFS
    Pos expandDirection;
    std::vector<Pos> ownFields;
    int age;
    int lifeTime;
    int hue;
    int saturation;
    int lightness;

    Branch(const Pos& pos)
        : pos(pos),
          state("RUNNING"),
          mode("CITY"),
          expandDirection(0, 0),
          age(0),
          lifeTime(LIFE_TIME),
          hue(36),
          saturation(255),
          lightness(130)
    {
        ownFields.push_back(pos);
    }

    sf::Color getColor()
    {
        // HSL to RGB
        return sf::Color(120,120,0);
    }

    void createLine(const Pos& toPos, sf::RenderTexture& window)
    {
        // Draw line from current pos to new pos
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(2*SIZE*pos.x, 2*SIZE*pos.y), getColor()),
            sf::Vertex(sf::Vector2f(2*SIZE*toPos.x, 2*SIZE*toPos.y), getColor())
        };
        window.draw(line, 2, sf::Lines);

        // Update position and remember it
        pos = toPos;
        ownFields.push_back(pos);
    }
    void update(sf::RenderTexture& window)
    {
        if(this->mode == "CITY")
        {
        }

    }
};


class Application {
private:
    sf::RenderWindow window;
    sf::Clock clock;
    sf::RenderTexture dynamicBuffer;

public:
    Application() : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "City Generator") {
        window.setFramerateLimit(60);
    }

    void run() {
        initialize();

        // Main loop
        while (window.isOpen()) {
            processEvents();
            update(window);
        }
    }

private:
    void initialize()
    {
        cellCountX = WINDOW_WIDTH / SIZE / 2;
        cellCountY = WINDOW_HEIGHT / SIZE / 2;

        cells.resize(cellCountX * cellCountY, 0);

        branches.clear();

        for(int i = 0; i < START_BRANCHES; i++)
        {
            int randomIdx = rand() % cells.size();
            Pos randomPos = Pos::fromIdx(randomIdx);
            branches.push_back(std::make_shared<Branch>(randomPos));
        }

        if (!dynamicBuffer.create(WINDOW_WIDTH, WINDOW_HEIGHT)) {
            std::cerr << "Failed to create render texture!" << std::endl;
        }
    }

    void processEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
                else if (event.key.code == sf::Keyboard::R) {
                    // Reset simulation
                    initialize();
                }
                else if (event.key.code == sf::Keyboard::Space) {
                    // Toggle pause
                }
            }
        }
    }

    void update(sf::RenderWindow& window) {
        // No clearing of window; previous content stays visible
        // window.clear(); // Don't clear the window

        // Calculate delta time
        float deltaTime = clock.restart().asSeconds();

        // drawBackground(dynamicBuffer);

        // Update your simulation here
        auto it = branches.begin();
        while (it != branches.end()) {
            if ((*it)->state == "STOPPED") {
                it = branches.erase(it);
            } else {
                (*it)->update(dynamicBuffer);
                ++it;
            }
        }

        window.clear();
        sf::Sprite dynamicSprite(dynamicBuffer.getTexture());
        window.draw(dynamicSprite);

        // Display the newly drawn content
        window.display();
    }


    void render()
    {
        // Clear the window
        // window.clear(sf::Color::Black);

        // Draw your simulation here
        // Example: Draw a circle
        sf::CircleShape shape(50);
        shape.setFillColor(sf::Color::Green);
        shape.setPosition(WINDOW_WIDTH/2 - 50, WINDOW_HEIGHT/2 - 50);
        window.draw(shape);

        // Display what was drawn
        window.display();
    }
};

int main() {
    try {
        Application app;
        app.run();
    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
