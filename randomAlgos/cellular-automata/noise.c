// including thingys I need
#include <ncurses.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <math.h>

//I kinda need this for the winsize
struct winsize win_size;
int y;
int x;

// Random function for choosing if something is white or black
int noise_gen(int density);

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

    //actually making the noise now
    for (int y_pos = 0; y_pos < y; y_pos++)
    {
        for (int x_pos = 0; x_pos < x; x_pos++)
        {
            if (noise_gen(density) == 0)
            {
                mvprintw(y_pos, x_pos, "#");
            }
        }
    }
    // letting me see this before ending 
    getch();
    //DO NOT FORGET THIS
    endwin();
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