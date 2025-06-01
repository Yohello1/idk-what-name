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
const int START_BRANCHES = 15;
const float SPREAD_CHANCE = 0.75;

enum States
{
    CITY,
    LAND,
    STOPPED
};

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

    Pos operator+(const Pos& a) const
    {
        return Pos(a.x+x, a.y+y);
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
    if(position.y < 0 || position.y > cellCountY)
        return -1;
    else
        return cells[position.toIdx(position.x, position.y)];
}

Pos getRandDirection()
{
    // 0-250: up
    // 250-500: left
    // 500-750: right
    // 750-1000: down
    int val = getRand();
    if(val < 250)
    {
        return Pos(0,1);
    }
    else if (250 < val && val < 500)
    {
        return Pos(-1,0);
    }
    else if(500 < val && val < 750)
    {
        return Pos(1,0);
    }
    else
    {
        return Pos(0, -1);
    }
}

class Branch
{
public:
    Pos pos;
    enum States mode; // CITY == BFS, LAND == DFS
    Pos expandDirection;
    std::vector<int> ownFields;
    int age;
    int lifeTime;
    int hue;
    int saturation;
    int lightness;

    Branch(const Pos& pos,
           enum States state_in = CITY,
           const Pos& expand_in = Pos(0,0),
           int age_in = 0,
           int lifetime_in = LIFE_TIME,
           int hue_in = 120,
           int saturation_in = 120,
           int lightness_in = 120)
        : pos(pos),
          mode(state_in),
          expandDirection(expand_in),
          age(age_in),
          lifeTime(lifetime_in),
          hue(hue_in),
          saturation(saturation_in),
          lightness(lightness_in)
    {
        ownFields.push_back(pos.toIdx(pos.x, pos.y));
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
        ownFields.push_back(toPos.toIdx(toPos.x, toPos.y));
        if(cells[toPos.toIdx()] != 3)
            std::cout << "ERROR";
        cells[toPos.toIdx()] = 3;
    }
    void update(sf::RenderTexture& window)
    {
        if (this->mode == CITY)
        {
            std::cout << "CITY" << std::endl;
        }
        else if(this->mode == LAND)
        {
            std::cout << "LAND" << std::endl;
            float chanceOfGoingForward = getRand();
            if(chanceOfGoingForward > 250)
            {
                // Go in expansion direction
                // Otherwise backtrack and find a free space
                if(getCellData(pos+expandDirection) == 0)
                {
                    createLine(pos+expandDirection, window);
                }
                else
                {
                    this->mode = STOPPED;
                }
            }
            else
            {
                expandDirection = getRandDirection();
                if(getCellData(pos+expandDirection) == 0)
                {
                    createLine(pos+expandDirection, window);
                }
                else
                {
                    this->mode = STOPPED;
                }
            }
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
            branches.push_back(std::make_shared<Branch>(randomPos, LAND, getRandDirection()));
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
        float deltaTime = clock.restart().asSeconds();

        for(int i = 0; i < 5; i++)
        {
            int idx = (int)(getRand()*1000.0) % branches.size();
            if(branches[idx] == NULL)
                continue;
            branches[idx]->update(dynamicBuffer );
        }


        window.clear();
        sf::Sprite dynamicSprite(dynamicBuffer.getTexture());
        window.draw(dynamicSprite);
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
