// https://youtu.be/icZj67PTFhc?si=tiSwjb48qH9QkfMU
#include <array>
#include <chrono>
#include <queue>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstring>
#include <random>
#include <bits/stdc++.h>
#include <unistd.h>
#include <iomanip>
#include <cmath>

#define MAP_SIZE 128

struct Comparator {
    bool operator()(std::tuple<double, double, std::pair<int, int>>& t1, std::tuple<double, double, std::pair<int, int>>& t2) {
         return std::get<0>(t1) > std::get<0>(t2);
     }
 };

sf::RenderWindow window(sf::VideoMode(1024,1024), "SFML works!");
sf::Texture texture;
sf::Sprite unvisted, visited, start, end, path, wall;

std::array<std::array<int, MAP_SIZE>, MAP_SIZE> map;
// FCost(total cost), HCost(to start cost), cord
std::priority_queue<std::tuple<double, double, std::pair<int, int>>, std::vector<std::tuple<double, double, std::pair<int, int>>>, Comparator> nodes;
std::map<std::pair<int, int>, std::tuple<double, std::pair<int, int>>> parents;
bool drawn = false;

void drawMap();
void generateMap(int drunkards);
bool evaluateNeighbours(std::tuple<double, double, std::pair<int, int>> point, std::pair<int, int> end);
void drawPath(std::pair<int, int> start, std::pair<int, int> end);

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
    std::pair<int, int> start, end;
    {
        srand(time(0));
        generateMap(200);

        start.first  = (rand() %  MAP_SIZE);
        start.second = (rand() %  MAP_SIZE);
        end.first    = (rand() %  MAP_SIZE);
        end.second   = (rand() %  MAP_SIZE);

        map[start.first][start.second] = 2;
        map[end.first]  [end.second]   = 3;

        parents[std::make_pair(start.first,start.second)] = {0, {start.first, start.second}};
        evaluateNeighbours({0, (std::sqrt(std::abs(start.first - end.first) + std::abs(start.second - end.second))), {start.first, start.second}}, end);
    }

    bool found = false;


    int currentPoint = 0;
    auto startT = std::chrono::high_resolution_clock::now();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
             if (event.type == sf::Event::Closed)
                 window.close();
        }

        if(nodes.size() == 0)
        {
            found = true;
            std::cout << "NOTHING";
            break;
        }
        else if(found == true)
        {

            drawPath(start, end);
            drawMap();
            window.display();
            sleep(2);
            // 49 boids per one point?
            if(currentPoint < 50)
            {
                start.first  = (rand() %  MAP_SIZE);
                start.second = (rand() %  MAP_SIZE);

                while(!nodes.empty())
                {
                    nodes.pop();
                }
                parents.clear();

                // Clearing the map
                for(int xPos = 0; xPos < MAP_SIZE; xPos++)
                {
                    for(int yPos = 0; yPos < MAP_SIZE; yPos++)
                    {
                        if(map[xPos][yPos] == 1 || map[xPos][yPos] == 4 || map[xPos][yPos] == 2)
                        {
                            map[xPos][yPos] = 0;
                        }
                    }
                }

                parents[std::make_pair(start.first,start.second)] = {0, {start.first, start.second}};
                evaluateNeighbours({0, (std::sqrt(std::abs(start.first - end.first) + std::abs(start.second - end.second))), {start.first, start.second}}, end);
                map[start.first][start.second] = 2;
                std::cout << "Size " << nodes.size();

                found = false;
            }
        }
        else
        {
            found = evaluateNeighbours(nodes.top(), end);
            nodes.pop();
        }


        window.clear();
        drawMap();
        window.display();

        // sleep(0.05);
    }

    auto elapsedT = std::chrono::high_resolution_clock::now() - startT;
    long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsedT).count();
    std::cout << microseconds << std::endl;

    return 0;
}

void drawMap()
{
    for(int x_pos = 0; x_pos < MAP_SIZE; x_pos++)
    {
        for(int y_pos = 0; y_pos < MAP_SIZE; y_pos++)
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

            // Wall
            else if(map[x_pos][y_pos] > 4)
            {
                wall.setPosition(x_pos*8,y_pos*8);
                window.draw(wall);
            }

        }
    }}

// Drunkards refering to how many 'drunk' things will be spawned to create the map
void generateMap(int drunkards)
{
    for(int i = 0; i < drunkards; i++)
    {
        // generate the spawn point
        int x_pos = rand() % (MAP_SIZE-1), y_pos = rand() %  (MAP_SIZE-1);

        for(int j = 0; j < 50; j++)
        {
            map[x_pos][y_pos] = 5;

            // X 0 X
            // 3 # 1
            // X 2 X
            int randInt = rand() % 4;
            if(randInt == 0 && y_pos > 0)
            {
                y_pos -= 1;
            }

            if(randInt == 1 && x_pos <  MAP_SIZE)
            {
                x_pos += 1;
            }

            if(randInt == 2 && y_pos <  MAP_SIZE)
            {
                y_pos += 1;
            }

            if(randInt == 3 && x_pos > 0)
            {
                x_pos -= 1;
            }

        }
    }
}

bool evaluateNeighbours(std::tuple<double, double, std::pair<int, int>> point, std::pair<int, int> end)
{
    // Fetch the current neighbours
    // Their HCost (cost from start) is current node +1
    // GCost is euclidian distance from end point
    // add them to priority queue
    // FCost(total cost), HCost(distance to start), cord
    // Add parent to map
    // Also set to visited

    int endX = end.first;
    int endY = end.second;

    double HCost = std::get<1>(point);
    int x, y;
    {
        std::pair temp = std::get<2>(point);

        x = temp.first;
        y = temp.second;
    }

    // . 0 .
    // 2 X 3
    // . 1 .

    if((y-1) > -1)
    {
        double newHCost = HCost + 1;

        if(map[x][y-1] == 0)
        {
            map[x][y-1] = 1;
            double GCost = std::sqrt(std::pow(std::abs(endX - x),2) + std::pow(std::abs(endY - (y-1)), 2));
            double FCost = newHCost + GCost;
            nodes.push({FCost, newHCost, {x, (y-1)}});

            // Map
            // Does it alr exist?
            // if it equals 0, it does not exist
            if(parents.count({x, (y-1)}) == 0)
            {
                parents[std::make_pair(x,(y-1))] = {newHCost, {x, y}};
            }
            else if(std::get<0>(parents[std::make_pair(x, (y-1))]) > newHCost)
            {
                // If it does exist, and is lower than etc
                parents[std::make_pair(x,(y-1))] = {newHCost, {x, y}};
            }


        }
        else if(map[x][y-1] == 3)
        {
            parents[std::make_pair(x,(y-1))] = {newHCost, {x, y}};
            return true;
        }
    }

    // Below
    if((y+1) < MAP_SIZE)
    {
        double newHCost = HCost + 1;
        if(map[x][y+1] == 0)
        {
            map[x][y+1] = 1;
            double GCost = std::sqrt(std::pow(std::abs(endX - x),2) + std::pow(std::abs(endY - (y+1)), 2));
            double FCost = newHCost + GCost;
            nodes.push({FCost, newHCost, {x, (y+1)}});

            // Map
            // Does it alr exist?
            // if it equals 0, it does not exist
            if(parents.count({x, (y+1)}) == 0)
            {
                parents[std::make_pair(x,(y+1))] = {newHCost, {x, y}};
            }
            else if(std::get<0>(parents[std::make_pair(x, (y+1))]) > newHCost)
            {
                // If it does exist, and is lower than etc
                parents[std::make_pair(x,(y+1))] = {newHCost, {x, y}};
            }


        }
        else if(map[x][y+1] == 3)
        {
            parents[std::make_pair(x,(y+1))] = {newHCost, {x, y}};
            return true;
        }
    }

    // left
    if((x-1) > -1)
    {
        double newHCost = HCost + 1;
        if(map[x-1][y] == 0)
        {
            map[x-1][y] = 1;

            double GCost = std::sqrt(std::pow(std::abs(endX - (x-1)),2) + std::pow(std::abs(endY - y), 2));
            double FCost = newHCost + GCost;
            nodes.push({FCost, newHCost, {(x-1), y}});

            // Map
            // Does it alr exist?
            // if it equals 0, it does not exist
            if(parents.count({(x-1), y}) == 0)
            {
                parents[std::make_pair((x-1),y)] = {newHCost, {x, y}};
            }
            else if(std::get<0>(parents[std::make_pair((x-1),y )]) > newHCost)
            {
                // If it does exist, and is lower than etc
                parents[std::make_pair((x-1), y)] = {newHCost, {x, y}};
            }
        }
        else if(map[x-1][y] == 3)
        {
            parents[std::make_pair((x-1), y)] = {newHCost, {x, y}};
            return true;
        }
    }

    // right
    if((x+1) < MAP_SIZE)
    {
        double newHCost = HCost + 1;
        if(map[x+1][y] == 0)
        {
            map[x+1][y] = 1;
            double GCost = std::sqrt(std::pow(std::abs(endX - (x+1)),2) + std::pow(std::abs(endY - y), 2));
            double FCost = newHCost + GCost;
            nodes.push({FCost, newHCost, {(x+1), y}});

            // Map
            // Does it alr exist?
            // if it equals 0, it does not exist
            if(parents.count({(x+1), y}) == 0)
            {
                parents[std::make_pair((x+1),y)] = {newHCost, {x, y}};
            }
            else if(std::get<0>(parents[std::make_pair((x+1),y )]) > newHCost)
            {
                // If it does exist, and is lower than etc
                parents[std::make_pair((x+1), y)] = {newHCost, {x, y}};
            }


        }
        else if(map[x+1][y] == 3)
        {
            parents[std::make_pair((x+1), y)] = {newHCost, {x, y}};
            return true;
        }
    }

    return false;

}

void drawPath(std::pair<int, int> start, std::pair<int, int> end)
{
    // Until we find the start point, keep going to parents?

    std::pair<int, int> nextPair = end;
    bool donePath = false;
    int i = 0;

    while(!donePath)
    {
        nextPair = std::get<1>(parents[std::make_pair(nextPair.first, nextPair.second)]);

        map[nextPair.first][nextPair.second] = 4;
        i++;

        if((nextPair.first == start.first) && (nextPair.second == start.second))
        {
            donePath = true;
        }

    }

}
