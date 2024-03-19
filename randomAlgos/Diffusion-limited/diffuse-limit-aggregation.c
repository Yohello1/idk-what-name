#include <ncurses.h>
#include <sys/ioctl.h>
#include <time.h>
#include <stdlib.h>
struct winsize win_size;
unsigned int currenttime;
int noise_run_times = 0;
int y_pos;
int x_pos;
int walkers_max;
int dir_walk(void);
int main()
{
    printf("How many walkers do you want there to be? ");
    scanf("%i", &walkers_max);
    initscr();
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_RED);
    ioctl(0, TIOCGWINSZ, &win_size);
    currenttime = (unsigned int)time(NULL);
    int x = win_size.ws_col;
    int y = win_size.ws_row;
    mvprintw(y / 2, x / 2, "#");
    refresh();
    getch();
    int points = 0;
    int dir;
    for (int walkers = 0; walkers < walkers_max; walkers++)
    {
        y_pos = rand() % win_size.ws_row;
        x_pos = rand() % win_size.ws_col;
        for (int steps = 0; steps < 1000000; steps++)
        {
            mvprintw(2, 3, "Walkers = %i", walkers);
            mvprintw(3, 3, "steps = %i", steps);
            mvprintw(4, 3, "Points = %i", points);

            int dir = dir_walk();
            // X 1 X
            // 4 * 2
            // X 3 X
            if (dir == 1)
            {
                y_pos--;
            }
            else if (dir == 2)
            {
                x_pos++;
            }
            else if (dir == 3)
            {
                y_pos++;
            }
            else
            {
                x_pos--;
            }
            move(y_pos, x_pos);
            // getch();

            if (mvinch(y_pos + 1, x_pos) == '#')
            {
                mvprintw(y_pos, x_pos, "#");
                points++;
                break;
            }
            else if (mvinch(y_pos - 1, x_pos) == '#')
            {
                mvprintw(y_pos, x_pos, "#");
                points++;
                break;
            }
            else if (mvinch(y_pos, x_pos + 1) == '#')
            {
                mvprintw(y_pos, x_pos, "#");
                points++;
                break;
            }
            else if (mvinch(y_pos, x_pos - 1) == '#')
            {
                mvprintw(y_pos, x_pos, "#");
                points++;
                break;
            }

            if (y_pos > win_size.ws_row)
            {
                break;
            }
            else if (x_pos > win_size.ws_col)
            {
                break;
            }
            else if (x_pos < 0)
            {
                break;
            }
            else if (y_pos < 0)
            {
                break;
            }
            // getch();
        }
        // getch();
    }
    refresh();

    getch();
    getch();
    getch();
    getch();
    getch();
    getch();
    getch();
    getch();
    getch();
    getch();
    getch();

    endwin();
}

int dir_walk(void)
{
    srand(currenttime + noise_run_times);
    noise_run_times++;
    int num = (rand() % 4);
    return num;
}
