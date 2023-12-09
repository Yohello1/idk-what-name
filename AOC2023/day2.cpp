#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <bits/stdc++.h>
#include <vector>

int main()
{
    std::cout << "hi" << std::endl;
    std::string line;
    std::ifstream myfile("input.txt");

    int sum = 0;
    int turtle_power = 0;

    // parse data!

    if(myfile.is_open())
    {
        while(std::getline(myfile, line))
        {
            // std::cout << line << std::endl;
            std::stringstream ss(line);
            std::vector<std::string> delimitedString;

            while(std::getline(ss, line, ' '))
            {
                delimitedString.push_back(line);

            }

            delimitedString.at(1).pop_back();

            int currentID = stoi(delimitedString.at(1));

            delimitedString.erase(delimitedString.begin());
            delimitedString.erase(delimitedString.begin());

            bool real = true;

            int redCubes = 0;
            int greenCubes = 0;
            int blueCubes = 0;

            while(true)
            {
                if(delimitedString.size() == 0)
                    break;
                else if(delimitedString.size() == 2)
                {
                    int val = stoi(delimitedString.at(0));

                    delimitedString.erase(delimitedString.begin());
                    std::string temp = delimitedString.at(0);

                    delimitedString.erase(delimitedString.begin());

                     if(temp.compare("red") == 0)
                        {
                            if(val > 12)
                                real = false;
                            redCubes = std::max(val, redCubes);
                        }

                    else if(temp.compare("green") == 0)
                        {
                            if(val > 13)
                                real = false;
                             greenCubes = std::max(val, greenCubes);
                        }
                    else if(temp.compare("blue") == 0)
                        {
                            if(val > 14)
                                real = false;

                            blueCubes = std::max(val, blueCubes);
                        }
                    else
                        {
                            std::cout << "ERROR";
                            return 0;
                        }

                }
                else
                {

                    int val = stoi(delimitedString.at(0));

                    delimitedString.erase(delimitedString.begin());
                    std::string temp = delimitedString.at(0);

                    temp.pop_back();
                    delimitedString.erase(delimitedString.begin());


                     if(temp.compare("red") == 0)
                        {
                            if(val > 12)
                                real = false;
                            redCubes = std::max(val, redCubes);
                        }

                    else if(temp.compare("green") == 0)
                        {
                            if(val > 13)
                                real = false;
                             greenCubes = std::max(val, greenCubes);
                        }
                    else if(temp.compare("blue") == 0)
                        {
                            if(val > 14)
                                real = false;

                            blueCubes = std::max(val, blueCubes);
                        }

                }

            }


            if(real == true)
            {

                 sum += currentID;
            }
            std::cout << "REDCUBESSS: " << redCubes << std::endl;
            turtle_power += redCubes*greenCubes*blueCubes;
        }
    }

    std::cout << sum << std::endl;
    std::cout << turtle_power << std::endl;

}
