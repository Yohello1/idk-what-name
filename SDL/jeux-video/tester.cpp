#include <iostream>
#include <algorithm>

int main() {
    int lo = 1;
    int hi = 99;
    int val = std::clamp(-3, lo, hi);
    std::cout << val << '\n';
}
