#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <map>

#ifndef SEED
 #define SEED 200
#endif

#ifndef FUNCS
 #define FUNCS 3
#endif

#ifndef LENGTH
 #define LENGTH 11
#endif

#ifndef ENDLEN
 #define ENDLEN 2
#endif

#ifndef CHANCE_OF_JMP
 #define CHANCE_OF_JMP 5
#endif


int main()
{
        // std::cout << "You are expected to of used -D to set the defines stuff, \n otherwise defaults will be used. \n This is problem 3's generator/solver" << std::endl;

        // choose random characters, add them to vector (can I put a vector into a struct?, yes, wtf is this)
        // that will be the func name
        // Then the struct will contain func length
        // Length will contain the 'sub' and 'end' parts
        // (a linked list would go crazy here, but ion have the energy to implment one :(
        // between `sub` and `end`, we literally just throw random stuff
        // if a rand between 0-5 == 2, we jmp to a random existent character
        // then the end is just jmp to an extenst character
        // wait how do i actually contain the data LMFAO
        //
        // Le guide de reccomendation!
        // Seed: self explainatory
        // Funcs: amount of functions
        // Length: TOTAL FUNCTION LENGTH!!!! as in, the amt of instructions output
        // EndLen: the length of the last two instructions, it doesnt output the last function lmao
        //        Like 2, means 1 actual funciton thrown out
        // CHANCE_OF_JMP: how common are jmps
        // btw, (length-endlen) has to be divisible by FUNCS, with a result of =>3, or it will break stuff

        srand(SEED);

        int independentSize = (LENGTH - ENDLEN)/FUNCS;

        // wait this could(?) be exploited

        std::map<char, std::pair<char, std::pair<int, std::vector<std::string>>>> existentFunctions; // this is redundant
        std::vector<char> nameFuncs;


        std::cout << LENGTH-1 << '\n';

        for(int i = 0; i < FUNCS; i++)
        {
                char temp;
                int attempts = 0;
                while(true)
                {
                        attempts++;
                        if(attempts == 100)
                                return 0; // welp gg go next
                        temp = 'a' + rand()%26;
                        if(existentFunctions.count(temp) == 0)
                                break;

                }

                nameFuncs.push_back(temp);

                existentFunctions[temp].first = temp;
                existentFunctions[temp].second.first = independentSize;
                // existentFunctions[temp].second.second;
                // when i profiled my game engine, rand() took up the most compute time LMFAO

                // rand() % 5
                // if != 4 out
                // if == 4 jmp

                int innerFuncLength = existentFunctions[temp].second.first - 2;

                std::string startFuncString = "sub ";
                startFuncString.push_back(existentFunctions[temp].first);
                existentFunctions[temp].second.second.push_back(startFuncString);

                for(int i = 0; i < innerFuncLength; i++)
                {
                    int valRand = rand() % CHANCE_OF_JMP;
                    if((valRand == 4) && (existentFunctions.size() != 1))
                    {
                        int weirdVal = rand() % nameFuncs.size();
                        char tempa = nameFuncs[weirdVal];
                        std::string jmpFuncString = "jmp ";
                        jmpFuncString.push_back(tempa);
                        existentFunctions[temp].second.second.push_back(jmpFuncString);
                    }
                    else
                    {
                        std::string outFuncString = "out ";
                        int weirdVal = rand() % 100;
                        outFuncString.append(std::to_string(weirdVal));
                        existentFunctions[temp].second.second.push_back(outFuncString);
                    }
                }

                std::string endFuncString = "end";
                existentFunctions[temp].second.second.push_back(endFuncString);
        }

        for (auto const& [key, val] : existentFunctions)
        {
                // std::cout << key << ':' << val.first << " " << val.second.second[0] << std::endl;
                for(int i = 0; i < (int) val.second.second.size(); i++)
                {
                    std::cout << val.second.second[i] << '\n';
                }
        }


        // now to make the end!
        // uhhh take end, and rand it!!!
        for(int i = 0; i < ENDLEN - 1; i++)
        {
                int randVal = rand() % CHANCE_OF_JMP;
                if(randVal == 3)
                {
                    std::cout << "jmp ";
                    char tempp = nameFuncs[rand() % nameFuncs.size()];
                    std::cout << tempp << '\n';
                }
                else
                {
                    std::cout << "out " << rand() % 100 << '\n';
                }
        }
}
