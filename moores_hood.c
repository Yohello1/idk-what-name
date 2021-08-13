// including thingys I need
#include <ncurses.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//I kinda need this for the winsize
struct winsize win_size;
int y;
int x;

// Random function for choosing if something is white or black
int noise_gen(int density);
int moores_hood(int y_pos, int x_pos);
void bomb(char *message);

// Making the buffer
static char buffer[] = "foobar";

#define FILENAME "window.dat"
int main()
{
    //getting density
    int density;
    scanf("%i", &density);

    //making window pointer
    WINDOW *win;
    FILE *wfile;
    FILE *newscreen;

    int r;

    //starting the screen
    initscr();
    win = newwin(5, 20, 7, 30);
    if (win == NULL)
        bomb("Unable to create window\n");
    // hiding the thingy, cursor visablity
    curs_set(0);

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
    getch();
    wrefresh(win);
    if (win == NULL)
        bomb("Unable to create window2\n");
    wfile = fopen("OLD.txt", "w+");
    if (wfile == NULL)
        bomb("Error creating file\n");
    else
        addstr("works\n");
    if (win == NULL)
        bomb("Unable to create window3\n");
    putwin(win, wfile);

    int walled;
    for (int i = 0; i < 50; i++)
    {
        for (int y_pos = 0; y_pos < y; y_pos++)
        {
            for (int x_pos = 0; x_pos < x; x_pos++)
            {

                moores_hood(y_pos, x_pos);
                refresh();
            }
        }
    }


    getch();
    endwin();

    //displaying debug info
    printf("diemensions: %i, %i \n", x, y);
    // printf("is 3,3 walled? %i ( 1 = yes, 0 = no) \n", walled);
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

int moores_hood(int y_pos, int x_pos)
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

    //finally switching it, time to go experimental
    if (walled < 4)
    {
        mvprintw(y_pos, x_pos, "#");
    }
    else
    {
        mvprintw(y_pos, x_pos, " ");
    }
    return walled;
}

void bomb(char *message)
{
    endwin();
    puts(message);
    exit(1);
}

// /* open the file */
// wfile = fopen(FILENAME, "r+");
// if (wfile == NULL)
//     bomb("Error reading file\n");

// /* write the window's data */
// win = getwin(wfile);
// if (win == NULL)
//     bomb("Unable to read/create window\n");
// fclose(wfile);
// wrefresh(win);
// getch();

// endwin();
// return 0;
// }