#include <bits/stdc++.h>
#include <vector>
#include <algorithm>
#include <cmath>

#define INF 1e9

std::vector<std::string> lines;
std::map<char, int> lightHeavyIndex;

int n = 0;

void heavyChars(int index)
{
    // search each character, get the value from map, and then, add to it
    for(int i = 0; i < n; i++)
    {
        char currentChar = lines.at(index).at(i);
        lightHeavyIndex[currentChar] += 1;
    }
}

// check if alternating
bool checkAlternating(int index)
{
    bool alternating = true;
    for(int i = 0; i < n-1; i++)
    {
        char currentChar = lines.at(index).at(i);
        char nextChar = lines.at(index).at(i+1);

        // true if heavy
        bool currentCharType = false, nextCharType = false;

        if(lightHeavyIndex[currentChar] > 1)
        {
            currentCharType = true;
        }

        if(lightHeavyIndex[nextChar] > 1)
        {
            nextCharType = true;
        }

        if(nextCharType == currentCharType)
            alternating = false;

    }

    return alternating;
}

int main()
{
    int t;
    std::cin >> t;
    std::cin >> n;
    for(int i = 0; i < t; i++)
    {
        std::string temp;
        std::cin >> temp;
        lines.push_back(temp);
    }

    // ok so, we first figure out what is light versus heavy
    // and then we just figure out if it is alternating or not

    std::string output = "";

    for(int i = 0; i < t; i++)
    {
        heavyChars(i);
        bool temp = checkAlternating(i);

        if(temp == false)
            output.append("F\n");
        else
            output.append("T\n");
        lightHeavyIndex.clear();
    }

    std::cout << output;

    // while(lightHeavyIndex.size() != 0)
    // {
    //     lightHeavyIndex[0].erase;
    // }


}
