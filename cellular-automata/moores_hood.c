// including thingys I need
#include <ncurses.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <time.h>
#include <unistd.h>

//I kinda need this for the winsize
struct winsize win_size;
int y;
int x;

// Random functions
int noise_gen(int density);
void moores_hood(int y_pos, int x_pos);
void bomb(char *message);
void shift_win_left(void);

// Size of a window, or like 1/2 of the screen
int win_size_x;

// For noise generation
unsigned int currenttime;
int noise_run_times;

int main()
{
    //getting density
    int density;
    printf("Enter Desity of the noise thingy");
    scanf("%i", &density);

    // Getting time
    currenttime = (unsigned int)time(NULL);

    //starting the screen
    initscr();

    //This is how I get the size getyx doesn't work well lol
    ioctl(0, TIOCGWINSZ, &win_size);
    y = win_size.ws_row;
    x = win_size.ws_col;

    // Getting 1/2 the size
    if ((y % 2) == 0)
    {
        win_size_x = x / 2;
    }
    else
    {
        win_size_x = (x - 1) / 2;
    }



    curs_set(0);
    refresh();
    getch();

    //actually making the noise now
    for (int y_pos = 0; y_pos < y; y_pos++)
    {
        for (int x_pos = 0; x_pos < win_size_x; x_pos++)
        {
            if (noise_gen(density) == 0)
            {
                mvprintw(y_pos, x_pos, "#");
            }
            else
            {
                mvprintw(y_pos, x_pos, " ");
            }
        }
    }

    //Refresh and making sure they know what is happening
    getch();
    refresh();

    for (int i = 0; i < 5; i++)
    {
        getch();
        for (int y_pos = 0; y_pos < y; y_pos++)
        {
            for (int x_pos = 0; x_pos < win_size_x; x_pos++)
            {
                // if (noise_gen(density) == 0)
                // {
                moores_hood(y_pos, x_pos);
                // }
            }
            // wrefresh(win2);
        }
        getch();
        shift_win_left();
    }

    mvprintw(0, 0, "The end is nye");
    getch();

    /*
You know what?
Screw this, Ima try another method
Have 2 windows, 
1 old, 1 new, an
switch which one is being shown when doing the analyaztion and printing
solves all the issues
*/

    getch();
    endwin();

    //displaying debug info
    printf("diemensions: %i, %i \n", x, y);
}

int noise_gen(int density)

{

    srand(currenttime + noise_run_times);
    noise_run_times++;
    int num = (rand() % 100);

    if (num > density)
    {
        num = 1;
    }
    else
    {
        num = 0;
    }
    return num;
}

void moores_hood(int y_pos, int x_pos)
{
    int walled = 0;
    //perpendicular stuff
    // X 1 X
    // 4 Y 3
    // X 2 X
    if (mvinch(y_pos + 1, x_pos) == '#')
    {
        walled++;
    }
    else if (mvinch(y_pos - 1, x_pos) == '#')
    {
        walled++;
    }
    else if (mvinch(y_pos, x_pos + 1) == '#')
    {
        walled++;
    }
    else if (mvinch(y_pos, x_pos - 1) == '#')
    {
        walled++;
    }

    // Adjacent
    //  2 X 3
    //  X X X
    //  4 X 1
    else if (mvinch(y_pos + 1, x_pos + 1) == '#')
    {
        walled++;
    }
    else if (mvinch(y_pos - 1, x_pos - 1) == '#')
    {
        walled++;
    }
    else if (mvinch(y_pos - 1, x_pos + 1) == '#')
    {
        walled++;
    }
    else if (mvinch(y_pos + 1, x_pos - 1) == '#')
    {
        walled++;
    }
    // wrefresh(win2);
    // // refresh();
    // wrefresh(win2);

    if (walled < 5)
    {
        mvprintw(y_pos, x_pos + win_size_x, "#");
    }
    else
    {
        mvprintw(y_pos, x_pos + win_size_x, " ");
    }
}

void shift_win_left()
{
    for (int x_pos = 0; x_pos < win_size_x; x_pos++)
    {
        for (int y_pos = 0; y_pos < y; y_pos++)
        {
            //read from 1 position and write to the equvilent across the screen
            char grid_temp = mvinch(y_pos, x_pos + win_size_x);
            mvprintw(y_pos, x_pos, "%c", grid_temp);
        }
    }
}

void bomb(char *message)
{
    endwin();
    puts(message);
    exit(1);
}
