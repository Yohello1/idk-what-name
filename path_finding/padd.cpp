#include <iostream>
#include <iomanip>
#include <sstream>

int main()
{
    // std::cout << std::setw (10) << "hi" << std::endl;

    int i = 17435;
    std::stringstream ss;
    ss << std::setw(10) << std::setfill('0') << i;
    std::string s = ss.str();

    std::cout << s << std::endl;
}
