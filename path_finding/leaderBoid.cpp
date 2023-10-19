#include <array>
#include <chrono>
#include <queue>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstring>
#include <random>
#include <cmath>
#include <thread>
#include <unistd.h>

#define MAP_SIZE 128
#define BIRD_AMT 400


class bird;
sf::RenderWindow window(sf::VideoMode(1024,1024), "SFML works!");
sf::Texture texture;
sf::Sprite unvisted, visited, start, end, path, wall;

float maxSpeed = 4, minSpeed = 3;
double centeringFactor = 0.5;
double matchingFactor = 1;
double turnFactor = 5;
double steeringFactor = 5;
double avoidanceFactor = 0.4;

std::array<std::array<int, 128>, 128> map;
std::pair<int, int> cursor;

class bird
{
    public:
    std::pair<float, float> cord;
    std::pair<float, float> velocity;
    std::vector<int> birdsToEval;


    void birdsInRange(std::array<bird, BIRD_AMT> boids, int range)
    {
        while(!birdsToEval.empty())
        {
            birdsToEval.clear();
        }

        for(int i = 0; i < BIRD_AMT; i++)
        {
            double distance = ((std::pow(std::abs(cord.first - boids[i].cord.first), 2)) +  (std::pow(std::abs(cord.second - boids[i].cord.second), 2)));
            if(distance < range)
            {
                // std::cout << "Bordy in range uwu" << " " << i << std::endl;
                birdsToEval.push_back(i);
            }
        }
    }

    void cohesionBirds(std::array<bird, BIRD_AMT> boids)
    {
        float xAvgPos = 0, xDist;
        float yAvgPos = 0, yDist;

        bool calcVal = false;

        for(int i = 0; (uint) i < birdsToEval.size(); i++)
        {
            xAvgPos += boids[birdsToEval.at(i)].cord.first;
            yAvgPos += boids[birdsToEval.at(i)].cord.second;
        }

        if(xAvgPos > 0)
        {
            xAvgPos = xAvgPos / birdsToEval.size();
            xDist = xAvgPos - cord.first;
            calcVal = true;
        }

        if(yAvgPos > 0)
        {
            yAvgPos = yAvgPos / birdsToEval.size();
            yDist = yAvgPos - cord.second;
            calcVal = true;
        }

        if(calcVal == true)
        {
            velocity.first  += xDist*centeringFactor;
            velocity.second += yDist*centeringFactor;
        }
    }

    void seperationBirds(std::array<bird, BIRD_AMT> boids)
    {
        // first find birds within X radius
        birdsInRange(boids, 100);

        float xAvgPos = 0, xDist = 0;
        float yAvgPos = 0, yDist = 0;

        bool calcVal = false;

        for(int i = 0; (uint) i < birdsToEval.size(); i++)
        {
            xAvgPos += boids[birdsToEval.at(i)].cord.first;
            yAvgPos += boids[birdsToEval.at(i)].cord.second;
        }

        if(xAvgPos > 0)
        {
            xAvgPos = xAvgPos / birdsToEval.size();
            xDist = xAvgPos - cord.first;
            calcVal = true;
        }

        if(yAvgPos > 0)
        {
            yAvgPos = yAvgPos / birdsToEval.size();
            yDist = yAvgPos - cord.second;
            calcVal = true;
        }

        if(calcVal == true)
        {
            velocity.first  += xDist*avoidanceFactor*-1;
            velocity.second += yDist*avoidanceFactor*-1;
        }

        // std::cout << "SIZE: " << birdsToEval.size() <<  std::endl;
    }

    void alignBirds(std::array<bird, BIRD_AMT> boids)
    {
        // get avg velocity
        float xVelAvg = 0;
        float yVelAvg = 0;

        for(int i = 0; (uint) i < birdsToEval.size(); i++)
        {
            xVelAvg += boids[birdsToEval.at(i)].velocity.first;
            yVelAvg += boids[birdsToEval.at(i)].velocity.second;
        }

        if(birdsToEval.size() > 0)
        {
            xVelAvg /= birdsToEval.size();
            yVelAvg /= birdsToEval.size();
        }

        if(birdsToEval.size() > 0)
        {
            velocity.first += (xVelAvg - velocity.first)*matchingFactor;
            velocity.second += (yVelAvg - velocity.second)*matchingFactor;
        }
    }

    void screenEdges()
    {
        if(cord.first < 5)
            velocity.first += turnFactor;
        if(cord.first > (MAP_SIZE - 5))
            velocity.first -= turnFactor;
        if(cord.second < 5)
            velocity.second += turnFactor;
        if(cord.second > (MAP_SIZE - 5))
            velocity.second -= turnFactor;
    }

    void collisionAvoidance()
    {

    }

    void seekLeader(std::pair<int, int> pointD)
    {
        std::pair<int, int> desiredVelocity = {std::min(pointD.first-cord.first, maxSpeed), std::min(pointD.second-cord.second, maxSpeed)};
        std::pair<int, int> steering = {desiredVelocity.first - velocity.first, desiredVelocity.second - velocity.second};
        velocity.first += steering.first/steeringFactor;
        velocity.second += steering.second/steeringFactor;

    }

    void update(std::array<bird, BIRD_AMT> boids)
    {
        float yChange = velocity.first;
        float xChange = velocity.second;
        if((0 < (xChange + cord.first))  && ( xChange + cord.first < (MAP_SIZE-1)))
        {
            cord.first += velocity.first;
        }
        if((0 < (yChange + cord.second))  && ( yChange + cord.second < (MAP_SIZE-1)))
        {
            cord.second += velocity.second;
        }

        velocity.first = 0;
        velocity.second = 0;

        birdsInRange(boids, 500);

        // cohesionBirds(boids);
        // alignBirds(boids);
        seekLeader(cursor);
        //collisionAvoidance();
        screenEdges();

        // NOTE: SEPERATION MUST BE RUN AT THE END!!!
        seperationBirds(boids);

        float speed = std::sqrt(std::pow(velocity.first,2) + std::pow(velocity.second,2));
        if(speed > maxSpeed)
        {
            velocity.first  = velocity.first/speed*maxSpeed;
            velocity.second = velocity.second/speed*maxSpeed;
        }

        if(speed < maxSpeed)
        {
            velocity.first = velocity.first/speed*minSpeed;
            velocity.second = velocity.second/speed*minSpeed;
        }


    }
};

std::array<bird, BIRD_AMT> boids;

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
        srand(time(0));
        generateMap(100);
        std::pair<int, int> start = {(rand() % 128), (rand() % 128)};
        map[start.first][start.second] = 2;
        map[rand() % 128][rand() % 128] = 3;

        closeOpenSpaces(start);
        generateBoids();

        for(int i = 0; i < BIRD_AMT; i++)
        {
            boids[i].birdsInRange(boids, BIRD_AMT);
            boids[i].velocity = {rand() % 2, rand() % 2};
            // boids[i].update(boids);
        }

    }

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            sf::Vector2i position = sf::Mouse::getPosition(window);
            cursor.first = position.x/8;
            cursor.second = position.y/8;
            // std::cout << "POS: { " << cursor.first << " , " << cursor.second << " }" << std::endl;
        }
        window.clear();

        for(int i = 0; i < BIRD_AMT; i++)
        {
            // std::cout << "ran " << boids[i].velocity << std::endl;
            boids[i].update(boids);
        }

        // drawMap();
        drawBoids();
        window.display();
        // std::this_thread::sleep_for(std::chrono::milliseconds(1000/60));
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
    for(int i = 0; i < BIRD_AMT; i++)
    {

        bool foundBoids = false;
        std::pair<int, int> point = {(rand() % 128), (rand() % 128)};
        while(foundBoids != true)
        {
            if(map[point.first][point.second] == 0)
            {
                boids[i].cord.first = point.first;
                boids[i].cord.second = point.second;

                // boids[i].rotation = (rand() % 360);
                // boids[i].velocity = (rand() % 5);
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
    sf::CircleShape shape(2);
    shape.setFillColor(sf::Color(13,114,214));

    for(int i = 0; i < BIRD_AMT; i++)
    {
        // std::cout << "Drawing point " << boids[i].cord.first*8 << " " <<  boids[i].cord.second*8 << std::endl;
        shape.setPosition(boids[i].cord.first*8, boids[i].cord.second*8);
        window.draw(shape);
    }
}
