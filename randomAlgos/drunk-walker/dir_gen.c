#include <time.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    int noise_run_times = 15;
    int currenttime = (unsigned int)time(NULL);

    for (int i = 0; i < 20; i++)
    {
        srand(currenttime + noise_run_times);
        noise_run_times++;
        printf("%i\n", (rand() % 4));
    }
}