#include <iostream>
#include <cmath>
#include <stdint.h>
#include <cmath>
#include <algorithm>
#include <numeric>

uint64_t calculateDeterminite(uint64_t i, uint64_t j, uint64_t k, uint64_t l)
{
    uint64_t returnVal = std::abs((long long) ((i*k-i*l)+(j*k-j*l)+(j*l-k*k)));

    // returnVal = std::abs(returnVal);
    return returnVal;
}

uint64_t factor(uint64_t value)
{
    // basically just go till primes LMAO
    unsigned int sprimes[16] = {0x02,0x03,0x05,0x07,0x0B,0x0D,0x11,0x13,
                                0x17,0x1D,0x1F,0x25,0x29,0x2B,0x2F,0x35};

    int counter = 0;
    while(counter < 16)
    {
        if(value % sprimes[counter] == 0)
        {
            value /= sprimes[counter];
        }
        else
        {
            counter++;
        }
    }

    return value;
}

int GCD(uint64_t a, uint64_t b)
{
   if (b == 0)
   return a;
   return GCD(b, a % b);
}

uint64_t calcModulus(uint64_t* list, int length)
{
    // GCD stands for greatest common demoninator, which results in whole number
    uint64_t* tempGCD = new uint64_t[length];
    for(int i = 0; i < length; i++)
    {
        tempGCD[i] = list[i];
    }

    for(int k = 1; k < length; k++)
    {
        for(int i = 0; i < length-k; i++)
        {
            tempGCD[i] = std::gcd(tempGCD[i], tempGCD[i+1]);
            // I did not know the gcd function was built in LMAO
        }
    }

    return factor(tempGCD[0]);
}

uint64_t getModulus(uint64_t * detlist, int length)
{
    uint64_t* tempGCD = new uint64_t[length];
    for(int i = 0;i<length;i++)
    {
        tempGCD[i] = detlist[i];
    }
    for(int k = 1;k<length;k++)
    {
        for(int i = 0;i<length-k;i++)
        {
            tempGCD[i] = std::gcd(tempGCD[i],tempGCD[i+1]);
            std::cout << tempGCD[i] << ' ';
        }
    }
    return factor(tempGCD[0]);
}



int main()
{
    // pre requisite values
    int outputCount = 8;
    int determiniteListLength = outputCount - 3;
    uint64_t* lcgOutput = new uint64_t[outputCount];
    uint64_t* determiniteList = new uint64_t[determiniteListLength];
    uint64_t modulus;
    uint64_t a, k , seed;
    std::cout << "Creating LCG values" << std::endl;

    // LCG basic
    uint64_t result = 12345;
    for (int i = 0; i < outputCount; i++)
    {
        // idfk what these values are called, otherwise i'd #define them LMAO
        result = 16806*result+78124;
        result %= 46585162;
        lcgOutput[i] = result;
        std::cout << result << std::endl;
    }

    // ok now the solver :))
    std::cout << "starting the solver" << std::endl;

    for(int i = 0; i < determiniteListLength; i++)
    {
        determiniteList[i] = calculateDeterminite(lcgOutput[i], lcgOutput[i+1], lcgOutput[i+2], lcgOutput[i+3]);
    }

    // output determinteList
    std::cout << '\n' << "determiniteList" << std::endl;
    for(int i = 0; i < determiniteListLength; i++)
    {
        std::cout << determiniteList[i] << std::endl;
    }

    // Modulus calc
    std::cout << '\n' << "Getting Modulus" << std::endl;
    modulus = getModulus(determiniteList, determiniteListLength);
    std::cout << modulus << std::endl;

}
