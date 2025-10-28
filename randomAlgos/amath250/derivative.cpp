#include <iostream>
#include <cmath>

int main()
{
    std::cout << "Enter the position you want the derivartive for, for the equation x^2+3" << std::endl;
    
    float val;
    std::cin >> val;

    double t_delta = 0.0001;
    std::cout << (-1*(std::pow(val, 2)+3)+(std::pow((val+t_delta),2)+3))/t_delta << std::endl;

}
