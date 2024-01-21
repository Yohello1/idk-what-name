#include <iostream>
#include <cmath>
#include <ctime>
#include <unistd.h>
#include <vector>
#include <algorithm>

#ifndef PPLC
 #define PPLC 20
#endif

#ifndef CERTC
 #define CERTC 5
#endif

#ifndef NAME_MAX
 #define NAME_MAX 32
#endif

#ifndef HEIGHT_MAX
 #define HEIGHT_MAX 32000
#endif

#ifndef SEED
 #define SEED 200
#endif

struct person
{
    std::string name;
    int grade = 0;
    int height = 0;
};


bool sortFunction(const person& a, const person& b)
{
    // grade
    if(a.grade < b.grade) return true;
    if(a.grade > b.grade) return false;

    // height
    if(b.height < a.height) return true;
    if(b.height > a.height) return false;

    // if same we proced, name
    if(a.name.compare(b.name) > 0) return true;
    if(a.name.compare(b.name) < 0) return false;

    return false; // how would it get here
}

std::string gen_random(const int len)
{
    static const char alphanum[] =
        "abcdefghijklmnopqrstuvwxyz"; // capital letters makes it vv confusing lmao
    std::string tmp_s;
    tmp_s.reserve(len);

    for (int i = 0; i < len; ++i)
    {
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    return tmp_s;
}



int main()
{
    std::cout << "You are expected to of used -D to set the defines stuff, \n otherwise defaults will be used. \n This is problem 2's generator/solver" << std::endl;
    srand(SEED);

    std::cout << PPLC << ' ' << CERTC << '\n';


    std::vector<person> people; // std::array CANNOT be used in a solution :P


    // make random names, and height
    for(int i = 0; i < PPLC; i++)
    {
        struct person temp;
        temp.name = gen_random((rand() % (NAME_MAX)) + 1);
        temp.grade = rand() % 12; // yes u can be grade 0
        temp.height = rand() % HEIGHT_MAX; // yes u can be 0 cm tall
        people.push_back(temp);
    }

    // writting it out
    for(int i = 0; i < PPLC; i++)
    {
        std::cout << people.at(i).name << ' ' << people.at(i).grade << ' ' << people.at(i).height << '\n';
    }

    std::sort(people.begin(), people.end(), sortFunction);
    std::cout << "SORTED " << std::endl << std::endl << std::endl << std::endl;

    for(int i = 0; i < PPLC; i++)
    {
        std::cout << people.at(i).name << ' ' << people.at(i).grade << ' ' << people.at(i).height << '\n';
    }



}
