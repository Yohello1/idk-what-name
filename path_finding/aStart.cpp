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

 struct aStarComparator {
    bool operator()(std::tuple<int, int, int>& t1, std::tuple<int, int, int>& t2) {
         return std::get<0>(t1) > std::get<0>(t2);
     }
 };


sf::RenderWindow window(sf::VideoMode(1024,1024), "SFML works!");
sf::Texture texture;
sf::Sprite unvisted, visited, start, end, path, wall;

std::array<std::array<int, 128>, 128> map;
// fCost, x, y
std::priority_queue<std::tuple<int,int,int>, std::vector<std::tuple<int, int, int>>, aStarComparator> openNodes; // Nodes to be searched
std::map<std::pair<int, int>, std::pair<int, int>> closedNodes; // Nodes that have been searched
bool temp;

void drawMap();
void generateMap(int drunkards);
void openNext(std::pair<int, int> start, std::pair<int, int> end);
bool evaluateSurrondings(std::tuple<int, int, int> point, std::tuple<int, int, int> start, std::tuple<int, int, int> end);

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
    std::pair<int, int > start, end;
    {
        srand(time(0));
        generateMap(100);

        start = {(rand() % 128), (rand() % 128)};
        end = {(rand() % 128), (rand() % 128)};

        std::cout << "stardIn: " << start.first << ' '  << start.second << std::endl;
        std::cout << "stardIn: " << end.first << ' '  << end.second << std::endl;

        map[start.first][start.second] = 2;
        map[end.first][end.second] = 3;

        //std::cout << "No seg fault yet" << std::endl;

        // pathFindBFS(start, end);

        openNodes.push({0, start.first, start.second});
        // openSurrondings(openNodes.top());
        evaluateSurrondings(openNodes.top(), {0, start.first, start.second}, {0, end.first, end.second});
        openNodes.pop();
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
        openNext(start, end);
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

void openNext(std::pair<int, int> start, std::pair<int, int> end)
{
    if((openNodes.size() != 0) && (temp != true))
    {
        temp = evaluateSurrondings(openNodes.top(),{0, start.first, start.second}, {0, end.first, end.second} );
        openNodes.pop();
    }
    else
    {
        return;
    }
}

bool evaluateSurrondings(std::tuple<int, int, int> point, std::tuple<int, int, int> start, std::tuple<int, int, int> end)
{
    int x = std::get<1>(point);
    int y = std::get<2>(point);

    int startX = std::get<1>(start);
    int startY = std::get<2>(start);
    int endX   = std::get<1>(end);
    int endY   = std::get<2>(end);
    // Distance = sqrt(abs(x1-x2) + abs(y1-y2))

    // Above
    if( ( y - 1 ) > -1)
    {
        if(map[x][y-1] == 0)
        {
            map[x][y-1] = 1;

            // HCost : start, GCost : end
            int HCost = std::sqrt(std::abs(startX - (x)) + std::abs(startY - (y-1)));
            int GCost = std::sqrt(std::abs(endX - (x))   + std::abs(endY   - (y-1)));
            int FCost = HCost + GCost;

            openNodes.push({FCost, x, (y-1)});
        }
        else if(map[x][y-1] == 3)
        {
            return true;
        }
    }

    // Below
    if( ( y + 1 ) < 128)
    {
        if(map[x][y+1] == 0)
        {
            map[x][y+1] = 1;

            // HCost : start, GCost : end
            int HCost = std::sqrt(std::abs(startX - (x)) + std::abs(startY - (y+1)));
            int GCost = std::sqrt(std::abs(endX   - (x)) + std::abs(endY   - (y+1)));
            int FCost = HCost + GCost;

            openNodes.push({FCost, x, (y+1)});
        }

        else if(map[x][y+1] == 3)
        {
            return true;
        }
    }

    // Left
    if( ( x - 1 ) > -1 )
    {
        if(map[x-1][y] == 0)
        {
            map[x-1][y] = 1;

            // HCost : start, GCost : end
            int HCost = std::sqrt(std::abs(startX - (x - 1)) + std::abs(startY - (y)));
            int GCost = std::sqrt(std::abs(endX   - (x - 1)) + std::abs(endY   - (y)));
            int FCost = HCost + GCost;

            openNodes.push({FCost, (x-1), y});
        }

        else if(map[x-1][y] == 3)
        {
            return true;
        }
    }

    // right
    if( ( x + 1 ) < 128 )
    {
        if(map[x+1][y] == 0)
        {
            map[x+1][y] = 1;

            // HCost : start, GCost : end
            int HCost = std::sqrt(std::abs(startX - (x + 1)) + std::abs(startY - (y)));
            int GCost = std::sqrt(std::abs(endX   - (x + 1)) + std::abs(endY   - (y)));
            int FCost = HCost + GCost;

            openNodes.push({FCost, (x+1), y});
        }

        else if(map[x+1][y] == 3)
        {
            return true;
        }
    }

    return false;
}


// I don't rlly need this LMAO
std::string pad3Digit(int number)
{
    std::stringstream ss;
    ss << std::setw(3) << std::setfill('0') << number;
    std::string s = ss.str();
    return s;
}

std::string merge2Strings(std::string firstString, std::string secondString)
{
    return (firstString + secondString);
}
