#include <glm/glm.hpp>
#include <iostream>
#include <cmath>
#include <vector>
#include <cstdint>

#ifndef SEED
 #define SEED 200
#endif

#ifndef SPHE // amount of spherees
 #define SPHE 200
#endif

#ifndef MAX_RAD // Largest radius
 #define MAX_RAD 10
#endif

#ifndef XY_MAX
 #define XY_MAX 10
#endif

// Bounding box will be assumed to be 0-10, x&y, z will be 10000
// Ray can be in any direction

std::vector<std::pair<glm::vec3, std::uint8_t>> spheres;

int solveVal(glm::vec3 rO, glm::vec3 rD)
{
    float mass = 0;

    for(std::size_t i = 0; i < spheres.size(); i++)
    {
        float tempDotPro = glm::dot(spheres[i].first-rO, rD);
        glm::vec3 closestPoint = rO + rD*tempDotPro;
        float lengthClose = glm::length(spheres[i].first - closestPoint);

        if(lengthClose < spheres[i].second)
        {
            float halfXTRelation = glm::sqrt(spheres[i].second*spheres[i].second - lengthClose*lengthClose);
            float XTRelation = halfXTRelation*2; // full distance travelled
            mass += XTRelation;
        }
        else
        {
            mass += 0;
        }

    }

    return ceil(mass);
}

int main()
{
    srand(SEED);

    // Generate ray
    glm::vec3 rayOrigin(rand() % XY_MAX, rand() % XY_MAX, rand() % XY_MAX);
    glm::vec3 rayDirect(rand() % XY_MAX, rand() % XY_MAX, rand() % XY_MAX);

    std::cout << "[" << rayOrigin.x << "," << rayOrigin.y << "," << rayOrigin.z << "]" << std::endl;
    std::cout << "[" << rayDirect.x << "," << rayDirect.y << "," << rayDirect.z << "]" << std::endl;



    // why did I think I need a vector to generate the spheres???
    // Generate spheres, outputted as [x,y,z,radius] all values as ints
    for(int i = 0; i < SPHE; i++)
    {
        glm::vec3 tempPos(rand() % XY_MAX, rand() % XY_MAX, rand() % XY_MAX);
        int tempVal = rand() % MAX_RAD ;
        std::cout << "[" << tempPos.x << "," << tempPos.y << "," << tempPos.z << "," << tempVal << "],";

        spheres.push_back(std::make_pair(tempPos, MAX_RAD));
    }

    std::cout << std::endl << solveVal(rayOrigin, rayDirect) << std::endl;
}
