#include <iostream>

int main()
{
    std::cout << "[a b\nc d]\nEneter in 'a b c d' order " << std::endl;
    double a,b,c,d;
    std::cin >> a >> b >> c >> d;

    std::cout << (a*d-b*c) << std::endl;
}
