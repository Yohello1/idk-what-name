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
    double distance = 0;
    int point;
    // Now we check the distance
    for (int y_cur_pos = 0; y_cur_pos < win_size.ws_row; y_cur_pos++)
    {
        for (int x_cur_pos = 0; x_cur_pos < win_size.ws_col; x_cur_pos++)
        {
            for (int i = 0; i < amount_of_points; i++)
            {
                y_dis = points[i].y_pos - y_cur_pos;
                x_dis = points[i].x_pos - x_cur_pos;
                float last_distance = sqrt(pow(y_dis, 2) + pow(x_dis, 2));

                if(last_distance < distance){
                    point = i;
                }
            }
            mvprintw(y_cur_pos, x_cur_pos, "%i", point);
            // getch();
        }
                    getch();

    }
                getch();

    endwin();
}