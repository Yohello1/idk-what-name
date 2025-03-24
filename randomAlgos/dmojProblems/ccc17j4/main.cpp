#include <iostream>
#include <vector>
#include <cstdint>
#include <cmath>

bool isSequence(std::vector<int> time)
{
    int a = 0;
    if((time[0] != 0))
        a = time[0] - time[1];
    else
        a = time[1] - time[2];

    if(!(time[1] - time[2] == a))
        return false;

    if(!(time[2] - time[3] == a))
        return false;

    return true;

}

void incrementTime(std::vector<int>& time)
{
    if(time[3] < 9)
        time[3] += 1;
    else if(time[2] < 5)
    {
        time[2] += 1;
        time[3] = 0;
    }
    else if(time[0] == 0 && time[1] < 9)
    {
        time[1] += 1;
        time[2] = 0;
        time[3] = 0;
    }
    else if(time[0] == 0 && time[1] == 9)
    {
        time = {1,0,0,0};
    }
    else if(time[0] == 1 && time[1] == 0 && time[2] == 5 && time[3] == 9)
    {
        time = {1,1,0,0};
    }

    else if (time[0] == 1 && time[1] == 1 && time[2] == 5 && time[3] == 9)
    {
        time = {0,0,0,0};
    }
    else if(time[0] == 1 && time[1] == 2 && time[2] == 5 && time[3] == 9)
    {
        time = {0,1,0,0};
    }

}

int main(int argc, char *argv[])
{
    long input;
    std::cin >> input;

    std::vector<int> time = {1,2,0,0};


    long long counter = 31*std::floor(input/720);
    input %= 720;

    for(int i = 0; i < std::min(input,(long)720); i++)
    {
        incrementTime(time);
        if(isSequence(time) == true)
        {
            counter++;
        }
        // std::cout << time[0] << "," << time[1] << "," << time[2] << "," << time[3] << std::endl;

    }

    std::cout << counter;




    return 0;
}
