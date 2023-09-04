#include <stdio.h>
#include <ncurses.h>
#include <sys/ioctl.h>
#include <unistd.h>

int main(void)
{
    printf("HELLO WORLLLDDD\n");

    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    printf ("lines %d\n", w.ws_row);
    printf ("columns %d\n", w.ws_col);

    if((w.ws_row < 100) || (w.ws_col < 100) )
    {
        printf("too small- that's what she said\n");
        return 0;
    }

}
