#include <iostream>
#include <NTL/ZZ_p.h>
#include <NTL/ZZ.h>
#include <NTL/lzz_p.h>

#include <NTL/tools.h>

/*
  Short way for calculating the determinant of a 3*3 matrix
*/
NTL::ZZ calcDet(NTL::ZZ val1, NTL::ZZ val2, NTL::ZZ val3, NTL::ZZ val4)
{
    return abs(val1*val3 - val1*val4 + val2*val3 - val2*val2 + val2*val4 - val3*val3);
}
/*
  Just to remove a few factors, the modulus might still
  multiplied by a small factor, like 2 or 10
*/
NTL::ZZ factor(NTL::ZZ value)
{
    unsigned int sprimes[16] = {0x02,0x03,0x05,0x07,0x0B,0x0D,0x11,0x13,
                                0x17,0x1D,0x1F,0x25,0x29,0x2B,0x2F,0x35};
    int counter = 0;
    while(counter < 16)
    {
        if(value % sprimes[counter] == 0) value /= sprimes[counter];
        else counter++;
    }
    return value;
}
/*
  Should do the trick to calculate the different GCDs of the determinants
*/
NTL::ZZ getModulus(NTL::ZZ * detlist, int length)
{
    NTL::ZZ * tempGCD = new NTL::ZZ[length];
    for(int i = 0;i<length;i++)
    {
        tempGCD[i] = detlist[i];
    }
    for(int k = 1;k<length;k++)
    {
        for(int i = 0;i<length-k;i++)
        {
            tempGCD[i] = GCD(tempGCD[i],tempGCD[i+1]);
        }
    }
    return factor(tempGCD[0]);
}

/*
  Short way to solve the 2 equations
*/
void linsolve(NTL::ZZ val1, NTL::ZZ val2, NTL::ZZ val3, NTL::ZZ modulus, NTL::ZZ &retval1, NTL::ZZ &retval2)
{
    NTL::zz_p::init(NTL::to_int(modulus));
    NTL::zz_p temp1 = NTL::to_zz_p(val1);
    NTL::zz_p temp2 = NTL::to_zz_p(val2);
    NTL::zz_p temp3 = NTL::to_zz_p(val3);
    retval1 = rep((temp2-temp3)/(temp1-temp2));
    retval2 = NTL::rep(temp2-(NTL::to_zz_p(retval1)*temp1));
}
/*
  This code calculates the Seed of the LCG if nth number is known
  Just a cheap reversal of the LCG
*/
NTL::ZZ calcSeed(NTL::ZZ a, NTL::ZZ k, NTL::ZZ m, NTL::ZZ nr, int nth)
{
    NTL::ZZ_p::init(m);
    NTL::ZZ_p retval;
    retval = NTL::to_zz_p(nr);
    for(int i = nth-1;i>=0;i--)
    {
        retval -=  NTL::to_zz_p(k);
        retval /=  NTL::to_zz_p(a);
    }
    return NTL::rep(retval);
}
int main()
{
    int outputcount = 8;
    int detlistlength = outputcount - 3;
    NTL::ZZ * lcgoutput = new NTL::ZZ[outputcount]; // lcgoutput contains
// the collected consecutive lcg output values
    NTL::ZZ * detlist = new NTL::ZZ[detlistlength];
    NTL::ZZ modulus;
    NTL::ZZ a,k,seed;
    std::cout << "Using the LCG values: " << std::endl;
/* Basic Linear Congruential Generator */
    int64_t result = 12345;
    for(int i = 0;i<8;i++)
    {
        result = 16807*result+78125;
        result %= 2147483647;
        lcgoutput[i] = result;
        std::cout << result << " ";
    }
/* ------------------------------------*/
    std::cout << std::endl;
    for(int i = 0;i<detlistlength;i++)
    {
        detlist[i] = calcDet(lcgoutput[i],lcgoutput[i+1],lcgoutput[i+2],lcgoutput[i+3]);
    }
    modulus = getModulus(detlist,detlistlength);
    for(int i = 0;i<outputcount;i++)
    {
        if(modulus < lcgoutput[i])
        {
            std::cout << "Modulus is too small (" << modulus <<"), probably no LCG" << std::endl;
            exit(-1);
        }
    }
    linsolve(lcgoutput[0],lcgoutput[1],lcgoutput[2],modulus,a,k);
    std::cout << "The LCG function is : x[n] = ( " << a;
    std::cout << "*x[n-1] + " << k << " ) % " << modulus << std::endl;
    seed = calcSeed(a,k,modulus,to_NTL::ZZ(lcgoutput[outputcount-1]),outputcount);
    std::cout << "The Seed used was: " << seed << std::endl;
    return 0;
}
