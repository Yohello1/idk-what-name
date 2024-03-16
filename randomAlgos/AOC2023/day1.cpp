#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

int main()
{
     std::string line;
     std::ifstream myfile ("input.txt");

     int sum = 0;

     if(myfile.is_open())
     {
       while(std::getline(myfile, line))
        {
            std::cout << line << std::endl;
            char* char_array = new char[line.length()+1];
            std::strcpy(char_array, line.c_str());

            char first, last = 'q';
            bool foundFirst = false;

            for(int i = 0; i < line.length(); i ++)
            {
                if(isdigit(char_array[i]) && foundFirst == false)
                {
                    foundFirst = true;
                    first = char_array[i];
                }
                else if(isdigit(char_array[i]))
                {
                    last = char_array[i];
                }
            }

            int val1 = (first - '0')*10;
            int val2 = 0;
            if(last == 'q')
            {
              val2 = (first - '0')*1;
            }
            else
            {
                val2 = (last - '0')*1;
            }

            sum += val1 + val2;
            std::cout << (val1 + val2) << std::endl;
        }
     }
     else
    {
         std::cout << "Hello world" << std::endl;
    }

    std::cout << sum << std::endl;
}
