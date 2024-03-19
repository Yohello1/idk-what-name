/**
 * @file update_funcs.hpp
 * @author Yohwllo
 * @brief where u put ur update functions to point to.
 * @version 0.1
 * @date 2023-01-26
 * 
 * @copyright Copyright (c) 2023
 * 
 */

// okie dokie lemme write out a quick functon which just does and,
namespace functions
{
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(0,255);

    // 1024 x 1024
    void fakeRandomImage(float *randImgOut, float shade)
    {
        for(int i = 0; i < 1024; i++)
        {
            for(int j = 0; j < 1024; j++)
            {
                for(int k = 0; k < 4; k++)
                {

                    // randImgOut[i*1024*4+j*4+k] = 0.1;
                    if(100 < i && i < 150)
                    {
                        if(300 < j && j < 724)
                            randImgOut[i*1024*4+j*4+k] = (rand() % 128)/64;
                    }
                    else if(874 < i && i < 924)
                    {
                        if(300 < j && j < 724)
                            randImgOut[i*1024*4+j*4+k] = (rand() % 128)/64;
                    }
                    else if(149 < i && i < 875)
                    {
                        if(462 < j && j < 562)
                            randImgOut[i*1024*4+j*4+k] = (rand() % 128)/64;
                    }
                }
            }
        }

    }

    void whiteSquares(float *randImgOut)
    {
        for(int i = 0; i < 1024; i++)
        {
            for(int j = 0; j < 1024; j++)
            {
                for(int k = 0; k < 4; k++)
                {
                    int current = 4096*i + 4*j + k;
                    if(current > 2097152 && current < 2105344)
                    {
                        // randImgOut[current] = 0.5;
                         randImgOut[current] = (float)(dist(mt)/255);
                    }
                }
            }
        }
    }
}
