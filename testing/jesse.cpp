#include <iostream>
#include <cmath>
#include <stdint.h>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <string>
#include <vector>
#include <set>

uint32_t seed, multiplier, moduloVal, addedVal;
uint32_t amtVals = 1000*6;
std::vector<uint32_t> values;
std::vector<uint32_t> absDeterminateVal;

bool isPrime(int num);
int32_t determinateOfMatrix(uint16_t position);
uint32_t largestFactorDeterminates();

int main(int argc, char *argv[])
{
    std::cout << "It is suggested that all numbers are primes" << std::endl;
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

    values.reserve(amtVals);
    values[0] = seed;
    for(uint32_t i = 1; i < amtVals;i++)
    {
        values[i] = (values[i-1]*multiplier + addedVal) % moduloVal;
    }

    absDeterminateVal.reserve(amtVals/6);
    for(uint32_t i = 0; i < amtVals/6; i+=6)
    {
        if(std::abs(determinateOfMatrix(i)) != 0)
        {
            absDeterminateVal.push_back(std::abs(determinateOfMatrix(i)));
            std::cout << std::abs(determinateOfMatrix(i)) << std::endl;
        }
    }

    std::cout << "Largest Factor " << std::endl;
    largestFactorDeterminates();
}

std::vector<uint32_t> intersection(std::vector<uint32_t> v1,
                                   std::vector<uint32_t> v2){
    std::vector<uint32_t> v3;

    std::sort(v1.begin(), v1.end());
    std::sort(v2.begin(), v2.end());

    std::set_intersection(v1.begin(),v1.end(),
                          v2.begin(),v2.end(),
                          back_inserter(v3));
    return v3;
}

uint32_t largestFactorDeterminates()
{
    uint32_t result = 0;

    std::set<uint32_t> factorPrev;
    std::set<uint32_t> factorCurr;
    std::set<uint32_t> factorNext;

    for(uint32_t i = 0; i < (uint32_t)absDeterminateVal.size() - 1; i++)
    {
        for(uint32_t j = 1; j < absDeterminateVal[i]; j++)
        {
            if((absDeterminateVal[i] % j) == 0)
                factorPrev.insert(j);
        }

        for(uint32_t j = 1; j < absDeterminateVal[i+1]; j++)
        {
            if((absDeterminateVal[i+1] % j) == 0)
                factorCurr.insert(j);
        }

        // insersect
        std::set_intersection(factorPrev.begin(), factorPrev.end(), factorCurr.begin(), factorCurr.end(), std::inserter(factorNext, factorNext.begin()));


        // wipe & prep
        factorPrev.clear();
        factorCurr.clear();
        factorPrev.insert(factorNext.begin(), factorNext.end());
        factorNext.clear();

    }

    std::cout << *factorPrev.rbegin();

    return result;
}

int32_t determinateOfMatrix(uint16_t position)
{
    // a1 a2 1
    // b1 b2 1
    // c1 c2 1
    //
    // x   x+1 1
    // x+2 x+3 1
    // x+4 x+5 1
    //
    // = a1(b2-c2) - a2(b1-c1) + (b1*c2-b2*c1)

    int32_t result = 0;

    uint32_t cal[6] = {values[position], values[position+1], values[position+2], values[position+3], values[position+4], values[position+5]};

    result = cal[0]*(cal[3]-cal[5]) - cal[1]*(cal[2]-cal[4]) + 1*(cal[2]*cal[5]-cal[3]*cal[4]);

    return result;
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
