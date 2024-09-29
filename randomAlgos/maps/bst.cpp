#include <iostream>

int pow(int base, int power);
{
    return base*
}

int powCurr(int power, int base)
{
    if(power == 0)
        return 1;
    return powCurr(power-1, base)*base;
}

int seed = time(NULL);
int lcg()
{
    seed = (seed*4789+23478)%pow(2,16);
}

typedef struct
{
    int val;
    node* left;
    node* right;
}node;

node root;

int main()
{
    root.val = lcg();
}
