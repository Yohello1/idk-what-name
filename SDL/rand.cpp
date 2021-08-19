#include <iostream>
#include <random>

int main(){
    const int seed = 45;
    std::mt19937_64 mtrand(seed);
    for(int i = 0; i < 20; i++){
        std::cout << mtrand() % 600 << "\n";
    }
}