#include <ncurses.h>
#include <time.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <string.h>
#define NORMAL 1
#define REMOVING 2
struct winsize win_size;
int noise_run_times = 0;
unsigned int currenttime;
int dir_walk(void);

int main()
{
    initscr();
    currenttime = (unsigned int)time(NULL);
    ioctl(0, TIOCGWINSZ, &win_size);

    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_RED);

    attron(COLOR_PAIR(NORMAL));
    //Fill screen with #
    for (int y_pos = 0; y_pos < win_size.ws_row; y_pos++)
    {
        for (int x_pos = 0; x_pos < win_size.ws_col; x_pos++)
        {
            mvprintw(y_pos, x_pos, "#");
        }
    }
    attroff(COLOR_PAIR(NORMAL));

    getch();

    mvprintw(win_size.ws_row / 2, win_size.ws_col / 2, ".");
    getch();

    int y_pos = win_size.ws_row / 2;
    int x_pos = win_size.ws_col / 2;

    for (int num_hulk = 0; num_hulk < 50; num_hulk++)
    {
        mvprintw(4, 3, "Hulks %i", num_hulk);
        mvprintw(1, 1, "#");

        for (int steps_till_die = 0; steps_till_die < 500; steps_till_die++)
        {
            int dir = dir_walk();

            // X 0 X
            // 3 P 1
            // X 2 X

            if (dir == 0)
            {
                attron(COLOR_PAIR(REMOVING));
                y_pos--;
                mvprintw(y_pos, x_pos, " ");
                attroff(COLOR_PAIR(REMOVING));
                getch();
                attron(COLOR_PAIR(NORMAL));
                mvprintw(y_pos, x_pos, ".");
                attroff(COLOR_PAIR(NORMAL));
            }
            else if (dir == 1)
            {
                attron(COLOR_PAIR(REMOVING));

                x_pos++;
                mvprintw(y_pos, x_pos, " ");

                attroff(COLOR_PAIR(REMOVING));
                getch();

                attron(COLOR_PAIR(NORMAL));

                mvprintw(y_pos, x_pos, ".");
                attroff(COLOR_PAIR(NORMAL));
            }
            else if (dir == 2)
            {
                attron(COLOR_PAIR(REMOVING));

                y_pos++;
                mvprintw(y_pos, x_pos, " ");

                attroff(COLOR_PAIR(REMOVING));
                getch();

                attron(COLOR_PAIR(NORMAL));

                mvprintw(y_pos, x_pos, ".");
                attroff(COLOR_PAIR(NORMAL));
            }
            else
            {
                attron(COLOR_PAIR(REMOVING));

                x_pos--;
                mvprintw(y_pos, x_pos, " ");

                attroff(COLOR_PAIR(REMOVING));
                getch();

                attron(COLOR_PAIR(NORMAL));

                mvprintw(y_pos, x_pos, ".");
                attroff(COLOR_PAIR(NORMAL));
            }
            mvprintw(3, 3, "Steps %i", steps_till_die);

            // Checking to see if it's out of bounds

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
        }

        for (int attempts = 0; attempts < 5000000; attempts++)
        {
            y_pos = rand() % win_size.ws_row;
            x_pos = rand() % win_size.ws_col;
            if (mvinch(y_pos, x_pos) == '.')
            {
                break;
            }
            mvprintw(4, 3, "attempts %i", attempts);
        }
    }

    getch();
    endwin();
}

int dir_walk(void)
{
    srand(currenttime * noise_run_times);
    noise_run_times++;
    int num = (rand() % 4);
    return num;
}