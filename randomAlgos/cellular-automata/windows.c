#include <ncurses.h>

int main()
{
    initscr();
    int height = 10, width = 20, start_y, start_x;
    start_y = start_x = 10;

    WINDOW * win = newwin(height, width, start_y, start_x);
    refresh();

    box(win, 0, 0);
    wrefresh(win);
     int c = getch();

     endwin();

     return 0;

}