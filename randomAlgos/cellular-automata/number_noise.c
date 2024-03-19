#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    int currenttime = (unsigned int)time(NULL);
    for (int i = 0; i < 100; i++)
    {
        srand(currenttime + i);
        printf("%i\n", rand() % 100);
    }
}