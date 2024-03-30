#include <array>
#include <chrono>
#include <queue>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstring>
#include <random>
#include <cmath>
#include <thread>
#include <fstream>
#include <unistd.h>

#define MAP_SIZE 1024

sf::RenderWindow window(sf::VideoMode(1024,1024), "SFML works!");


uint8_t map[MAP_SIZE][MAP_SIZE];
uint8_t future[MAP_SIZE][MAP_SIZE];

void drawMap();
void generateMap();
void gameOfLife();
void outputRegion(std::pair<int, int> left, std::pair<int, int> right);
void loadMap(std::string filepath, std::pair<int, int> startPoint);

int main()
{

    // Filling array
    {
        // map.fill(0);
        memset( &map[0][0], 0, sizeof(uint8_t)*MAP_SIZE*MAP_SIZE );
        memset( &future[0][0], 0, sizeof(uint8_t)*MAP_SIZE*MAP_SIZE );
    }

    // generate Map
    {
        srand(time(0));
        loadMap("block.in",{(MAP_SIZE/2 - MAP_SIZE/32), (MAP_SIZE/2 - MAP_SIZE/32)} );
        // generateMap();
    }


    for (int i = 0; i < 500; i++)
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();
        gameOfLife();
        //outputRegion({(MAP_SIZE/2 - MAP_SIZE/64), (MAP_SIZE/2 - MAP_SIZE/64)}, {(MAP_SIZE/2 + MAP_SIZE/64), (MAP_SIZE/2 + MAP_SIZE/64) });
        drawMap();
        window.display();
        // std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    return 0;
}

void drawMap()
{
    // define a 120x50 rectangle
    sf::RectangleShape white(sf::Vector2f(1, 1));
    white.setFillColor(sf::Color::White);
    sf::RectangleShape black(sf::Vector2f(1, 1));
    black.setFillColor(sf::Color::Black);
    for(int x_pos = 0; x_pos < MAP_SIZE; x_pos++)
    {
        for(int y_pos = 0; y_pos < MAP_SIZE; y_pos++)
        {

            // let's say
            // 0 = dead
            // 1 = alive

            // Unvisted
            if(map[x_pos][y_pos] == 0)
            {
                black.setPosition(x_pos,y_pos);
                window.draw(black);
            }

            // Visted
            else if(map[x_pos][y_pos] == 1)
            {
                white.setPosition(x_pos,y_pos);
                window.draw(white);
            }
        }
    }
}

// Drunkards refering to how many 'drunk' things will be spawned to create the map
void generateMap()
{
    const int temp = 8;
    for(int i = (MAP_SIZE/2 - (MAP_SIZE/temp)); i <  (MAP_SIZE/2 + (MAP_SIZE/temp)); i++)
    {
        for(int j =  (MAP_SIZE/2 - (MAP_SIZE/temp)); j <  (MAP_SIZE/2 + (MAP_SIZE/temp)); j++)
        {
            int randInt = rand() % 3;
            if(randInt == 0)
                map[i][j] = 1;
            else
                map[i][j] = 0;

        }
    }
}

void loadMap(std::string filePath, std::pair<int, int> startPoint)
{
    std::ifstream myfile;
    myfile.open(filePath);

    std::string temp;
    int y_pos = startPoint.second;
    while(myfile)
    {
        std::getline(myfile, temp);
        // let's assume it's 64 chars long
        for(int i = 0; i < temp.length(); i++)
        {
            if(temp[i] == '1')
                map[startPoint.first+i][y_pos] = 1;
            else
                map[startPoint.first+i][y_pos] = 0;
        }

        y_pos++;
    }
}


void gameOfLife()
{
    for(int i = 0; i < MAP_SIZE; i++)
    {
        for(int j = 0; j < MAP_SIZE; j++)
        {
            uint8_t count = 0;
            // 0 1 2
            // 3 X 4
            // 5 6 7

            // one massive bounds check LMAO
            if((i != 0) && (j != 0) && (i != (MAP_SIZE-1)) && (j != (MAP_SIZE-1)))
            {
                count += map[i-1][j-1];
                count += map[i]  [j-1];
                count += map[i+1][j-1];

                count += map[i-1][j];
                count += map[i+1][j];

                count += map[i-1][j+1];
                count += map[i]  [j+1];
                count += map[i+1][j+1];
            }


            if(count == 0)
                future[i][j] = 0;
            else if(count == 1)
                future[i][j] = 0;
            else if(count == 2)
                future[i][j] = 1;
            else if(count == 3)
                future[i][j] = 1;
            else if(count > 3)
                future[i][j] = 0;

        }
    }

    memset( &map[0][0], 0, sizeof(future) );
    std::memcpy(&map, &future, sizeof(uint8_t)*MAP_SIZE*MAP_SIZE);
    memset( &future[0][0], 0, sizeof(future) );
}

void outputRegion(std::pair<int, int> left, std::pair<int, int> right)
{
    for(int i = left.first; i < right.first; i++)
    {
        for(int j = left.second; j < right.second; j++)
        {
            std::cout <<(int) map[i][j] << ' ';
        }
        std::cout << '\n';
    }
}
