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

sf::RenderWindow window(sf::VideoMode(1024,1024), "SFML works!");
sf::Texture texture;
sf::Sprite unvisted, visited, start, end, path, wall;

std::array<std::array<int, 128>, 128> map;
std::queue<std::pair<int,int>> openNodes;
std::set<std::pair<int,  int>> closedNodes;

void drawMap();
void generateMap(int drunkards);
void pathFindAStar(std::pair<int, int> start, std::pair<int, int> end);
void openNext();
void openSurrondings(std::pair<int,int> point);

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

        std::pair<int,int> start = {(rand() % 128), (rand() % 128)};
        std::pair<int,int> end = {(rand() % 128), (rand() % 128)};

        std::cout << "stardIn: " << start.first << ' '  << start.second << std::endl;
        std::cout << "stardIn: " << end.first << ' '  << end.second << std::endl;

        map[start.first][start.second] = 2;
        map[end.first][end.second] = 3;

        std::cout << "No seg fault yet" << std::endl;

        pathFindAStar(start, end);

        std::cout << "The issue" << std::endl;
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
        drawMap();
        window.display();

        sleep(0.01);
        openNext();
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
    }}

// Drunkards refering to how many 'drunk' things will be spawned to create the map
void generateMap(int drunkards)
{
    for(int i = 0; i < drunkards; i++)
    {
        // generate the spawn point
        int x_pos = rand() % 128, y_pos = rand() % 128;

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

            if(randInt == 1 && x_pos < 128)
            {
                x_pos += 1;
            }

            if(randInt == 2 && y_pos < 128)
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

void pathFindAStar(std::pair<int, int> start, std::pair<int, int> end)
{
    openNodes.push(start);

    openSurrondings(openNodes.front());
    openNodes.pop();

    // while(openNodes.size() > 0)
    // {


    // }

}

void openNext()
{
    if(openNodes.size() != 0)
    {
        openSurrondings(openNodes.front());
        openNodes.pop();
    }
    else
    {
        return;
    }
}

void openSurrondings(std::pair<int, int> point)
{
    int x = point.first;
    int y = point.second;

    // Above
    if( ( y - 1 ) > 0)
    {
        if(map[x][y-1] == 0)
        {
            map[x][y-1] = 1;
            openNodes.push({x, (y-1)});
        }
    }

    // Below
    if( ( y + 1 ) < 128)
    {
        if(map[x][y+1] == 0)
        {
            map[x][y+1] = 1;
            openNodes.push({x, (y+1)});
        }
    }

    // Left
    if( ( x - 1 ) > 0 )
    {
        if(map[x-1][y] == 0)
        {
            map[x-1][y] = 1;
            openNodes.push({(x-1), y});
        }
    }

    // Above
    if( ( x + 1 ) < 128 )
    {
        if(map[x+1][y] == 0)
        {
            map[x+1][y] = 1;
            openNodes.push({(x+1), y});
        }
    }
}
