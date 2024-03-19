#include <bits/stdc++.h>
#include <vector>
#include <algorithm>
#include <cmath>

#define INF 1e9

std::vector<int> hats;

int main()
{
    int n = 0;
    std::cin >> n;

    for(int i = 0; i < n; i++)
    {
        int temp;
        std::cin >> temp;
        hats.push_back(temp);
    }

    for(int i = 0; i < n; i++)
    {
        // std::cout << hats.at(i);
    }

    // compute half, multiply by 2
    int totalPpl = 0;
    int half = n/2;
    for(int i = 0; i < half; i++)
    {
        if(hats.at(i) == hats.at(i+half))
            totalPpl++;
    }


    std::cout << totalPpl*2;
}
