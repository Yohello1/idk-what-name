#include <iostream>
#include <cmath>
#include <stdint.h>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <string>
#include <vector>

uint64_t seed, multiplier, moduloVal, addedVal;
uint32_t amtVals = 4294967295;
std::vector<uint64_t> values;

bool isPrime(int num);

int main(int argc, char *argv[])
{
    std::cout << "It is suggested that the moduloVal is a prime number" << std::endl;
    // x_{k+1}  = g*x_{k} % m
    if(argc == 1)
    {
        std::cout << "You need to give us three space seperated integers as arguments, [seed] [multiplier] [modulo value]\n";
        return 0;
    }

    // Input
    {
        std::string strSeed(argv[1]);
        std::string strMultiplier(argv[2]);
        std::string strModuloValue(argv[3]);
        std::string strAddedVal(argv[4]);

        seed = std::stoul(strSeed, nullptr);
        multiplier = std::stoul(strMultiplier, nullptr);
        moduloVal = std::stoul(strModuloValue, nullptr);
        addedVal = std::stoul(strAddedVal, nullptr);

        std::cout << seed << ' ' << multiplier << ' ' << moduloVal << std::endl;

        if(!isPrime(moduloVal))
        {
            std::cout << "Modulo val is NOT a prime" << std::endl;
        }
    }


    // ok gonna compute 2^12 numbers (4096)
    values.reserve(amtVals);
    values[0] = seed;
    for(uint32_t i = 1; i < amtVals;i++)
    {
        values[i] = (values[i-1]*multiplier + addedVal) % moduloVal;
        // std::cout << values[i] << std::endl;
    }
}



bool isPrime(int num)
{
    bool flag = true;

    for(int i = 2; i <= std::sqrt(num); i++)
    {
        if(num % i == 0)
        {
            flag = false;
            break;
        }
    }
    return flag;
}
