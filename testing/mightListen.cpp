#include <iostream>
#include <stdint.h>
#include <vector>
#include <algorithm>
#include <set>




int main()
{

    std::vector<uint32_t> absDeterminateVal;
    for(int i = 0; i < 11; i++)
    {
        std::string temp;
        std::cin >> temp;
        uint32_t whack = std::stoul(temp, nullptr);
        absDeterminateVal.push_back(whack);
    }



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

    for (uint32_t const& person : factorPrev)
    {
        std::cout << person << ' ';
    }

    return result;
}
