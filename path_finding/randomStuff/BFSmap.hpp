std::queue<std::pair<int,int>> openNodesBFS;

bool BFSopenSurrondings(std::pair<int, int> point)
{
    int x = point.first;
    int y = point.second;

    // Above
    if( ( y - 1 ) > -1)
    {
        if(map[x][y-1] == 0)
        {
            map[x][y-1] = 1;
            openNodesBFS.push({x, (y-1)});
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
            openNodesBFS.push({x, (y+1)});
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
            openNodesBFS.push({(x-1), y});
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
            openNodesBFS.push({(x+1), y});
        }

        else if(map[x+1][y] == 3)
        {
            return true;
        }
    }

    return false;
}

void closeOpenSpaces(std::pair<int, int> start)
{
    openNodesBFS.push(start);
    BFSopenSurrondings(openNodesBFS.front());
    openNodesBFS.pop();

    while((openNodesBFS.size() > 0))
    {
        BFSopenSurrondings(openNodesBFS.front());
        openNodesBFS.pop();
        // std::cout << "Size: " << openNodesBFS.size() << std::endl;
    }


    for(int i = 0; i < MAP_SIZE; i++)
    {
        for(int j = 0; j < MAP_SIZE; j++)
        {
            if(map[i][j] == 0)
            {
                map[i][j] = 5;
            }
        }
    }

    for(int i = 0; i < MAP_SIZE; i++)
    {
        for(int j = 0; j < MAP_SIZE; j++)
        {
            if(map[i][j] == 1)
            {
                map[i][j] = 0;
            }
        }
    }

    while(!openNodesBFS.empty())
    {
        openNodesBFS.pop();
    }
}
