#include <iostream>
#include <cmath>

int main()
{
    std::cout << "enter the range in which to integrate x^2 + 4, for. First enter the start of the range, then hit enter, then enter in the end of the range, and hit enter" << std::endl;

    double start, end;
    std::cin >> start >> end;

    double t_delta = 0.00001;
    double sum = 0.0;

    for(double i = start; i < end; i += t_delta)
    {
        sum += (std::pow(i,2) + 4)*t_delta;
    }

    std::cout << sum << std::endl;
}
