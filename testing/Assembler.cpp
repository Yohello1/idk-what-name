#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

std::map<char, int> funcLookUp;
std::vector<std::pair<std::string, std::string >> funcTable;
std::vector<char> parents;

bool failure = false;

std::string followJmp(char funcName)
{
    // if its name is found in the parents, then kill itself
    if(parents.size() != 0)
    {
        std::vector<char>::iterator itt;
        itt = std::find(parents.begin(), parents.end(), funcName);
        if((itt != parents.end()))
        {
            // failure, we found ourselves
            failure = true;

            return "";
        }
    }

     parents.push_back(funcName);

    std::string expectedOutput;
    int currentLine = funcLookUp[funcName];
    for(int i = currentLine; funcTable[i].first != "end"; i++)
    {
        if(funcTable[i].first == "out")
        {
            expectedOutput.append(funcTable[i].second);
            expectedOutput.append("\n");
        }
        if(funcTable[i].first == "jmp")
        {
            // parents.push_back(funcTable[i].second[0]);
            std::string followUp = followJmp(funcTable[i].second[0]);
            expectedOutput.append(followUp);
        }

    }

    return expectedOutput;
}

int main()
{
        // std::cout << "Hellow orld" << std::endl;

        // solver for p3-
        // ok so
        // create map, using a character as a key, and using the value
        // the value being what line it is,
        // You have to skip to that line (located where sub is), and
        // keep itterating/outputting
        // recursive too!
        // just shut up and listennn

        int finalEnd = -1;

        {
            int n = 0;
            std::cin >> n;
            for(int i = 0; i < n; i++)
            {
                std::string temp;
                std::cin >> temp;
                if(temp == "end")
                {
                    std::pair<std::string, std::string> data;
                    data.first = temp;
                    data.second = "";
                    funcTable.push_back(data);
                    finalEnd = i;
                }
                else if(temp == "out")
                {
                    std::pair<std::string, std::string> data;
                    data.first = temp;
                    std::cin >> data.second;
                    funcTable.push_back(data);
                }
                else if(temp == "jmp")
                {
                    std::pair<std::string, std::string> data;
                    data.first = temp;
                    std::cin >> data.second;
                    funcTable.push_back(data);
                }
                else if(temp == "sub")
                {
                    std::pair<std::string, std::string> data;
                    data.first = temp;
                    std::cin >> data.second;
                    funcTable.push_back(data);
                    funcLookUp[data.second[0]] = i;
                }
                else
                {
                    std::cout << "ERROR" << std::endl;
                    return -1;
                }
            }

            for(int i = 0;  i < n; i++)
            {
                // std::cout << i << " " << funcTable[i].first << funcTable[i].second << std::endl;
            }
        }

        // if not getting fucked
        std::string output;

        for(int i = finalEnd; (i < (int) funcTable.size()) && (failure != true); i++)
        {
            // out, and then jmp func
            if(funcTable[i].first == "out")
            {
                output.append(funcTable[i].second);
                output.append("\n");
            }
            else if (funcTable[i].first == "jmp")
            {
                // parents.push_back(funcTable[i].second[0]);
                std::string followUp = followJmp(funcTable[i].second[0]);
                output.append(followUp);
                parents.clear();
            }
        }

       if(failure == true)
       {
           std::cout << "HALT" << std::endl;
       }
       else
       {
           std::cout << output;
       }

}
