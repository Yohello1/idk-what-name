#include <iostream>
#include <vector>
#include <glm/glm.hpp>

std::vector<std::pair<glm::vec3, std::pair<glm::vec3, glm::vec3>>> trianglePoints;
std::vector<std::pair<glm::vec3, glm::vec3>> vectors;
std::vector<glm::vec3> normals;


int main()
{
    struct Ray
    {
        glm::vec3 startPoint;
        glm::vec3 rayDirection;
        float t;
    };




}
