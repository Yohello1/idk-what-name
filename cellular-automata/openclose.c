#include <ncurses.h>
#include <stdlib.h>

#define FILENAME "window.dat"

void bomb(char *message);

int main(void)
{
    FILE *wfile;
    WINDOW *win;
    int r;

    initscr();
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLUE);

    addstr("Creating new window\n");
    refresh();

    /* Crete the window */
    win = newwin(5, 20, 7, 30);
    if (win == NULL)
        bomb("Unable to create window\n");
    wbkgd(win, COLOR_PAIR(1));
    mvwaddstr(win, 1, 2, "This program was\n");
    mvwaddstr(win, 2, 5, "created by\n");
    mvwaddstr(win, 3, 5, "Yohellooo\n"); /* put your name here */
    wrefresh(win);
    getch();

    /* open the file */
    wfile = fopen(FILENAME, "w");
    if (wfile == NULL)
        bomb("Error creating file\n");

    /* write the window's data */
    r = putwin(win, wfile);
    if (r == ERR)
        addstr("Error putting window to disk\n");
    else
        addstr("Window put to disk\n");
    fclose(wfile);
    refresh();
    getch();

    erase();

    addstr("Press Enter to read the window from disk:\n");
    refresh();
    getch();

    /* open the file */
    wfile = fopen(FILENAME, "r");
    if (wfile == NULL)
        bomb("Error reading file\n");

    /* write the window's data */
    win = getwin(wfile);
    if (win == NULL)
        bomb("Unable to read/create window\n");
    fclose(wfile);
    wrefresh(win);
    getch();
    
    endwin();
    return 0;
}

void bomb(char *message)
{
    endwin();
    puts(message);
    exit(1);
}
