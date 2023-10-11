#include <array>
#include <chrono>
#include <queue>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstring>
#include <random>
#include <cmath>

#define MAP_SIZE 128
#define BIRD_AMT 500

class bird;
sf::RenderWindow window(sf::VideoMode(1024,1024), "SFML works!");
sf::Texture texture;
sf::Sprite unvisted, visited, start, end, path, wall;

std::array<std::array<int, 128>, 128> map;
class bird
{
    public:
    std::pair<int, int> cord;
    int rotation = 0;
    int velocity = 0;
    int acceleration = 0;
    std::vector<int> birdsToEval;

    void update()
    {
        // ok do da monster math to figure out direction;
        // y = round((sin(Θ*180/pi)*vel.y))
        // x = round((cos(Θ*180/pi)*vel.y))

        rotation = rotation % 360;

        int yChange = std::round((std::sin(rotation*3.14/180)*velocity));
        int xChange = std::round((std::cos(rotation*3.14/180)*velocity));

        std::cout << "not yet failed" << std::endl;

        if((-1 < (xChange + cord.first))  && ( yChange + cord.second < MAP_SIZE))
        {
            if(map[(xChange + cord.first)][cord.second] != 5)
            {
                cord.first = xChange + cord.first;
            }
        }

        std::cout << "dead" << std::endl;

        if((-1 < (yChange + cord.second))  && ( yChange + cord.second < MAP_SIZE))
        {
            if(map[cord.first][cord.second + yChange] != 5)
            {
                cord.second = yChange + cord.second;
            }
        }

        std::cout << "dead p2" << std::endl;

        velocity += acceleration;

        std::cout << "velocity" << std::endl;
        
    }

    void birdsInRange(bird boids[], int range)
    {
        std::cout << "quebec" << std::endl;
        while(!birdsToEval.empty())
        {
            birdsToEval.clear();
        }

        std::cout << "montreal" << std::endl;

        for(int i = 0; i < BIRD_AMT; i++)
        {
            double distance = ((std::pow(std::abs(cord.first - boids[i].cord.first), 2)) +  (std::pow(std::abs(cord.second - boids[i].cord.second), 2)));
            std::cout << "protocol" << std::endl;
            if(distance < range)
            {
                std::cout << "paris" << std::endl;
                // std::cout << "Bordy in range uwu" << " " << i << std::endl;
                birdsToEval.push_back(3);

                std::cout << "wawa" << std::endl;
            }
        }
    }


    void cohesionBirds()
    {
        for(int i = 0; i < birdsToEval.size(); i++)
        {

        }
    }
};

bird boids[500];

void drawMap();
void generateMap(int drunkards);
void generateBoids();
void drawBoids();

#include "randomStuff/BFSmap.hpp"

int main()
{

    // Load texture
    {
        if(!texture.loadFromFile("randomStuff/tileSprite.png")) { std::cout << "file did not infact load" << std::endl; }
        texture.setSmooth(true);
    }

    // Setting sprites
    {
        unvisted.setTexture(texture);
        unvisted.setTextureRect(sf::IntRect(0, 0, 8, 8));
        // window.draw(unvisted);

        visited.setTexture(texture);
        visited.setTextureRect(sf::IntRect(8,0,8,8));

        start.setTexture(texture);
        start.setTextureRect(sf::IntRect(16,0,8,8));

        end.setTexture(texture);
        end.setTextureRect(sf::IntRect(24,0,8,8));

        path.setTexture(texture);
        path.setTextureRect(sf::IntRect(32,0,8,8));

        wall.setTexture(texture);
        wall.setTextureRect(sf::IntRect(40,0,8,8));
    }

    // Filling array
    {
        // map.fill(0);
        memset( &map[0][0], 0, sizeof(map) );
    }

    // placing start & end points
    {
        srand(100);
        generateMap(100);
        std::pair<int, int> start = {(rand() % 128), (rand() % 128)};
        map[start.first][start.second] = 2;
        map[rand() % 128][rand() % 128] = 3;

        closeOpenSpaces(start);
        generateBoids();

        for(int i = 0; i < BIRD_AMT; i++)
        {
            std::cout << "owo" << std::endl;

            boids[i].birdsInRange(boids, 500);

            std::cout << "french" << std::endl;

            boids[i].velocity = 3;

            std::cout << "uwu" << std::endl;

            boids[i].update();
        }

    }


    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();

        for(int i = 0; i < BIRD_AMT; i++)
        {
            // std::cout << "ran" << i << std::endl;
            // boids[i].update();
        }

        drawMap();
        drawBoids();
        window.display();
    }

    return 0;
}

void drawMap()
{
    for(int x_pos = 0; x_pos < 128; x_pos++)
    {
        for(int y_pos = 0; y_pos < 128; y_pos++)
        {

            // let's say
            // 0 = unvisted
            // 1 = visted
            // 2 = start
            // 3 = end
            // 4 = path
            // 5 = wall

            // Unvisted
            if(map[x_pos][y_pos] == 0)
            {
                unvisted.setPosition(x_pos*8,y_pos*8);
                window.draw(unvisted);
            }

            // Visted
            else if(map[x_pos][y_pos] == 1)
            {
                visited.setPosition(x_pos*8,y_pos*8);
                window.draw(visited);
            }

            // Start
            else if(map[x_pos][y_pos] == 2)
            {
                start.setPosition(x_pos*8,y_pos*8);
                window.draw(start);
            }

            // end
            else if(map[x_pos][y_pos] == 3)
            {
                end.setPosition(x_pos*8,y_pos*8);
                window.draw(end);
            }

            // Path
            else if(map[x_pos][y_pos] == 4)
            {
                path.setPosition(x_pos*8,y_pos*8);
                window.draw(path);
            }


            else if(map[x_pos][y_pos] == 5)
            {
                wall.setPosition(x_pos*8,y_pos*8);
                window.draw(wall);
            }

        }
    }
}

// Drunkards refering to how many 'drunk' things will be spawned to create the map
void generateMap(int drunkards)
{
    for(int i = 0; i < drunkards; i++)
    {
        // generate the spawn point
        int x_pos = rand() % 127, y_pos = rand() % 127;

        for(int j = 0; j < 50; j++)
        {
            map[x_pos][y_pos] = 5;

            // X 0 X
            // 3 # 1
            // X 2 X
            int randInt = rand() % 4;
            if(randInt == 0 && (y_pos-1) > 0)
            {
                y_pos -= 1;
            }

            if(randInt == 1 && (x_pos+1) < 128)
            {
                x_pos += 1;
            }

            if(randInt == 2 && (y_pos+1) < 128)
            {
                y_pos += 1;
            }

            if(randInt == 3 && (x_pos-1) > 0)
            {
                x_pos -= 1;
            }

        }
    }
}

void generateBoids()
{
    for(int i = 0; i < 500; i++)
    {

        bool foundBoids = false;
        std::pair<int, int> point = {(rand() % 128), (rand() % 128)};
        while(foundBoids != true)
        {
            if(map[point.first][point.second] == 0)
            {
                boids[i].cord.first = point.first;
                boids[i].cord.second = point.second;

                boids[i].rotation = (rand() % 360);
                boids[i].velocity = (rand() % 5);
                foundBoids = true;
            }
            else
            {
                point = {(rand() % 128), (rand() % 128)};
            }
        }
    }
}

// Note, must be drawn AFTER map is drawn :)
void drawBoids()
{
    for(int i = 0; i < 500; i++)
    {

        path.setPosition(boids[i].cord.first*8,boids[i].cord.second*8);
        window.draw(path);
    }
}
