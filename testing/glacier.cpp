#include <iostream>

int main()
{
    int width;
    int Ttime;
    int slipRate;

    // take input
    std::cin >> width;
    std::cin >> Ttime;
    std::cin >> slipRate;

    int sum = 0;
    for(int i = 0; i < ((width*width) - width*Ttime*slipRate); i++)
    {
        int temp = 0;
        std::cin >> temp;
        sum += temp;
    }

    if(slipRate*Ttime > width)
        width = 0;
        sum = 0;
    sum -= width*(width-slipRate*Ttime);
    if(sum < 1)
        sum = 0;
    std::cout << sum << std::endl;
}
