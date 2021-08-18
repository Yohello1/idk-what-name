#include <ncurses.h>
#include <time.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
struct winsize win_size;
unsigned int currenttime;
int amount_of_points = 10;

struct positions
{
    int point;
    int x_pos;
    int y_pos;
};

int main()
{
    currenttime = (unsigned int)time(NULL);
    ioctl(0, TIOCGWINSZ, &win_size);
    initscr();
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    init_pair(4, COLOR_BLUE, COLOR_BLACK);
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(6, COLOR_CYAN, COLOR_BLACK);
    init_pair(7, COLOR_WHITE, COLOR_BLACK);
    init_pair(8, COLOR_WHITE, COLOR_GREEN);
    init_pair(9, COLOR_MAGENTA, COLOR_CYAN);
    init_pair(10, COLOR_RED, COLOR_YELLOW);

    mvprintw(3, 2, " X = %i, Y = %i", win_size.ws_col, win_size.ws_row);

    struct positions points[amount_of_points];

    //placing points
    for (int i = 0; i < amount_of_points; i++)
    {
        srand(currenttime + i);
        points[i].point = i;
        points[i].x_pos = rand() % win_size.ws_col;
        points[i].y_pos = rand() % win_size.ws_row;
        mvprintw(3, 2, " X = %i, Y = %i", points[i].x_pos, points[i].y_pos);

        mvprintw(points[i].y_pos, points[i].x_pos, "#");
        getch();
    }
    getch();

    int y_dis;
    int x_dis;
    int updates = 0;
    // Now we check the distance
    for (int y_cur_pos = 0; y_cur_pos < win_size.ws_row; y_cur_pos++)
    {
        for (int x_cur_pos = 0; x_cur_pos < win_size.ws_col; x_cur_pos++)
        {
            int point;
            float distance = 100000;

            for (int i = 0; i < amount_of_points; i++)
            {
                y_dis = points[i].y_pos - y_cur_pos;
                x_dis = points[i].x_pos - x_cur_pos;
                float last_distance = sqrt(pow(y_dis, 2) + pow(x_dis, 2));
                // mvprintw(20, 3, "%.2f", last_distance);
                // mvprintw(21, 3, "%.2f", distance);

                if (last_distance < distance)
                {
                    distance = last_distance;
                    point = i;
                    // mvprintw(19, 3, "%i", point);
                    updates++;
                    // mvprintw(18, 3, "%i", updates);
                }
            }
            attron(COLOR_PAIR(point));
            mvprintw(y_cur_pos, x_cur_pos, "%i", point);
                        attroff(COLOR_PAIR(point));

            // getch();
        }
        // getch();
    }
    curs_set(1);
    for (int i = amount_of_points; i >= 0; i--)
    {
        mvprintw(points[i].y_pos, points[i].x_pos, "#");
    }
    getch();
    curs_set(3);

    endwin();
}