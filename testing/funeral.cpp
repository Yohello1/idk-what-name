#include <iostream>
#include <string>
#include <vector>
#include <stdint.h>
#include <cmath>

int main()
{
    std::vector<std::string> inputs;
    std::vector<uint32_t> cal;
    for(int i = 0; i < 6; i++)
    {
        std::string temp;
        std::cin >> temp;
        inputs.push_back(temp);
        cal.push_back(std::stoi(temp));
    }
    std::string whack;
    std::cin >> whack;

    int32_t determinate =  cal[0]*(cal[3]-cal[5]) - cal[1]*(cal[2]-cal[4]) + 1*(cal[2]*cal[5]-cal[3]*cal[4]);

    if(determinate < 0)
        determinate *= -1;

    // 0 1 x
    // 2 3 x
    // 4 5 x

    std::cout << "\\[\n";
    std::cout << "\\det(" << whack << ")\n";

    std::cout << ' ' << inputs[0] << " \\cdot\n";
    std::cout << "\\begin{vmatrix}\n";
    std::cout << inputs[3] << " & " << "1" << " \\\\\n";
    std::cout << inputs[5] << " & " << "1" << " \\\\\n";
    std::cout << "\\end{vmatrix}\n";
    std::cout << "-\n";

    std::cout << ' ' << inputs[1] << " \\cdot\n";
    std::cout << "\\begin{vmatrix}\n";
    std::cout << inputs[2] << " & " << "1" << " \\\\\n";
    std::cout << inputs[4] << " & " << "1" << " \\\\\n";
    std::cout << "\\end{vmatrix}\n";
    std::cout << "+\n";

    std::cout << "\\begin{vmatrix}\n";
    std::cout << inputs[0] << " & " << inputs[1] << " \\\\\n";
    std::cout << inputs[4] << " & " << inputs[5] << " \\\\\n";
    std::cout << "\\end{vmatrix}\n";
    std::cout << "\\]\n";

    std::cout << "\\[\n";
    std::cout << "\\det(" << whack << ") = " << determinate << '\n';
    std::cout << "\\]\n";



}
