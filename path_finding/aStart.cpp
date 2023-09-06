#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <time.h>
#include <ctime>
#include <iostream>
#include <bits/stdc++.h>

struct cord_2d
{
    int x;
    int y;
};
struct tile
{
    // G Cost, dist from start
    // H is distance from end
    int GCost, HCost;
    // 0: Undiscovered
    // 1: start
    // 2: end
    // 3: discovered
    int type;
    tile* parent;

    cord_2d whoami;
};


tile waterGunFight[100][100];
std::vector<tile *> listDiscovered;
bool discovered = false;

void aStarSearch();

int main(void)
{
    time_t t;
    printf("HELLO WORLLLDDD\n");
    srand((unsigned) time(&t));
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    printf ("lines %d\n", w.ws_row);
    printf ("columns %d\n", w.ws_col);
    printf("%s\n", "\x1b[32mtest\033[0ming");

    if((w.ws_row < 100) || (w.ws_col < 100) )
    {
        printf("too small- that's what she said\n");
        return 0;
    }

    initscr();
    start_color();
    init_pair(1,COLOR_RED, COLOR_RED);
    init_pair(2,COLOR_GREEN,COLOR_GREEN);
    init_pair(3,COLOR_WHITE,COLOR_BLACK);
    refresh();

    for(int y_pos = 0; y_pos < 100; y_pos++)
    {
        for(int x_pos = 0; x_pos < 100; x_pos++)
        {
            mvprintw(y_pos, x_pos, "#");
            waterGunFight[y_pos][x_pos].whoami.x = x_pos;
            waterGunFight[y_pos][x_pos].whoami.y = y_pos;
            waterGunFight[rand()%100][rand()%100].type = 0;
        }
    }

    // rand start & end positions lmao
    {
        waterGunFight[rand()%100][rand()%100].type = 1;
        waterGunFight[rand()%100][rand()%100].type = 2;
    }


    // Drawing map
    for(int y_pos = 0; y_pos < 100; y_pos++)
    {
        for(int x_pos = 0; x_pos < 100; x_pos++)
        {
            if(waterGunFight[y_pos][x_pos].type == 1)
            {
                attron(COLOR_PAIR(1));
                mvprintw(y_pos, x_pos, "#");
                attroff(COLOR_PAIR(1));
            }
            else if(waterGunFight[y_pos][x_pos].type == 2)
            {
                attron(COLOR_PAIR(2));
                mvprintw(y_pos, x_pos, "#");
                attroff(COLOR_PAIR(2));
            }

            else
            {
                attron(COLOR_PAIR(3));
                mvprintw(y_pos, x_pos, "#");
                attroff(COLOR_PAIR(3));
            }
        }
    }

    cord_2d startPoint, endPoint;

    // Find start point
    for(int y_pos = 0; y_pos < 100; y_pos++)
    {
        for(int x_pos = 0; x_pos < 100; x_pos++)
        {
            if(waterGunFight[y_pos][x_pos].type == 1)
            {
                startPoint.y = y_pos;
                startPoint.x = x_pos;
            }
            else if(waterGunFight[y_pos][x_pos].type == 2)
            {
                endPoint.y = y_pos;
                endPoint.x = x_pos;
            }
        }
    }

    listDiscovered.push_back(&waterGunFight[startPoint.y][startPoint.x]);
    while(discovered == false)
    {
        aStarSearch();
    }


    refresh();
    endwin();
    getch();

}

// I hope I dont fall asleep making this
void aStarSearch()
{
    int lowestT = 0; // Lowest total cost, sum of g & H
    tile* lowestTile;

    for(int i = 0; i < listDiscovered.size(); i++)
    {
        int tempX = listDiscovered.at(i)->whoami.x;
        int tempY = listDiscovered.at(i)->whoami.y;

        // End point heuristic
    }
}
