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
    void fakeRandomImage(float *randImgOut)
    {
        for(int i = 0; i < 1020; i++)
        {
            for(int j = 0; j < 1024; j++)
            {
                // randImgOut[i][j] = glm::vec4((float)dist(mt)/255,(float)dist(mt)/255,(float)dist(mt)/255, 1.0);
                // randImgOut[i][j].x = (float)dist(mt)/255;
                // randImgOut[i][j] = glm::vec4( glm::vec3( 0.0 ), 1.0 );
                // randImgOut[i][j] = glm::vec4(1.0);
                // randImgOut = glm::vec4(1.0);
                for(int k = 0; k < 4; k++)
                {
                    // std::cout << "val: " << i*1024+j*4+k << '\n';
                    randImgOut[i*1024+j*4+k] = (float)(dist(mt)/255);
                }
            }
        }
    }
}
