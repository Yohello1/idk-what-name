#include <bits/stdc++.h>

#ifndef AMT_T
 #define AMT_T 10
#endif

int main()
{
    srand(time(NULL));
    std::cout << "[";
    for(int i = 0; i < AMT_T; i++)
    {
        std::cout << "[" << rand() % 1000 << ',' <<  rand() % 100 << ']' << ',';
    }
    std::cout << "]" << std::endl;
}
