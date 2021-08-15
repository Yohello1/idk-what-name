// including thingys I need
#include <ncurses.h>
#include <stdio.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <time.h>

//I kinda need this for the winsize
struct winsize win_size;
int y;
int x;

static char buffer[] = "foobar";

// Random function for choosing if something is white or black
int noise_gen(int density);
void moores_hood(int y_pos, int x_pos);
void bomb(char *message);
void shift_win_left(void);
int win_offset;
int win_size_x;

int main()
{
    //getting density
    int density;
    scanf("%i", &density);

    //starting the screen
    initscr();

    //This is how I get the size getyx doesn't work well lol
    ioctl(0, TIOCGWINSZ, &win_size);
    y = win_size.ws_row;
    x = win_size.ws_col;

    if ((y % 2) == 0)
    {
        win_size_x = x / 2;
    }
    else
    {
        win_size_x = (x - 1) / 2;
    }

    win_offset = win_size_x;

    mvprintw(0, 0, "%i", win_size_x);

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
            mvprintw(4, 0, "%i", x_pos);
            mvprintw(5, 0, "%i", y_pos);
        }
    }

    getch();
    refresh();

    //closing window
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
                mvprintw(4, 0, "%i", x_pos);
                mvprintw(5, 0, "%i", y_pos);
            }
            // wrefresh(win2);
        }
        getch();
        shift_win_left();
    }

    mvprintw(0, 0, "test");

    mvprintw(0, 0, "The end is nye");
    getch();

    /*
You know what?
Screw this, Ima try another method
Have 3 windows, 
1 old, 1 new, and one being shown (We need the 3rd one so stuff stays realitively simple)
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

    int lower = 0, upper = 100;
    int num = (rand() % (upper - lower + 1)) + lower;
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
    if (mvinch(y_pos - 1, x_pos) == '#')
    {
        walled++;
    }
    if (mvinch(y_pos, x_pos + 1) == '#')
    {
        walled++;
    }
    if (mvinch(y_pos, x_pos - 1) == '#')
    {
        walled++;
    }

    // Adjacent
    //  4 X 1
    //  X X X
    //  2 X 3
    if (mvinch(y_pos + 1, x_pos + 1) == '#')
    {
        walled++;
    }
    if (mvinch(y_pos - 1, x_pos - 1) == '#')
    {
        walled++;
    }
    if (mvinch(y_pos - 1, x_pos + 1) == '#')
    {
        walled++;
    }
    if (mvinch(y_pos + 1, x_pos - 1) == '#')
    {
        walled++;
    }
    // wrefresh(win2);
    // // refresh();
    // wrefresh(win2);

    if (walled < 5)
    {
        mvprintw(y_pos, x_pos + win_offset, "#");
    }
    else
    {
        mvprintw(y_pos, x_pos + win_offset, " ");
    }
}

void shift_win_left()
{
    for (int x_pos = 0; x_pos < win_offset; x_pos++)
    {
        for (int y_pos = 0; y_pos < y; y_pos++)
        {
            char grid_temp = mvinch(y_pos, x_pos + win_offset);
            mvprintw(y_pos, x_pos, "%c" ,grid_temp);
        }
    }
}

void bomb(char *message)
{
    endwin();
    puts(message);
    exit(1);
}
