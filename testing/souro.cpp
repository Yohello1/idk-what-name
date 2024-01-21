#include <iostream>
#include <cstdint>
#include <cmath>

#ifndef SEED
 #define SEED 200
#endif

#ifndef WIDTH
 #define WIDTH 4
#endif

#ifndef TIME
 #define TIME 1
#endif

#ifndef SLIP_RATE
 #define SLIP_RATE 1
#endif

#ifndef MAX_VAL
 #define MAX_VAL 9
#endif

int randNumber(uint16_t max)
{
    return (rand() % max); // hol up this is like unessialry weird, anwyasy compiler!
}


int main()
{
    std::cout << "You are expected to of used -D to set the defines stuff, \n otherwise defaults will be used. \n This is problem 2's generator/solver" << std::endl;

    srand(SEED);

    std::cout << WIDTH << '\n';
    std::cout << TIME << ' ' << SLIP_RATE << '\n';

    // matrix maker!
    int matrix[WIDTH][WIDTH];

    for(int i = 0; i < WIDTH; i++)
    {
        for(int j = 0; j < WIDTH; j++)
        {
            matrix[i][j] = randNumber(MAX_VAL);
        }
    }

    for(int i = 0; i < WIDTH; i++)
    {
        for(int j = 0; j < WIDTH; j++)
        {
            std::cout << matrix[i][j] << ' ';
        }
        std::cout << '\n';
    }

    // sum the integers
    int sum = 0;
    for(int i = 0; i < (WIDTH - (TIME*SLIP_RATE)); i++)
    {
        for(int j = 0; j < WIDTH; j++)
        {
            sum += matrix[i][j];
        }
    }

    sum -= (WIDTH-(TIME*SLIP_RATE))*WIDTH;
    std::cerr << sum << std::endl;


}
