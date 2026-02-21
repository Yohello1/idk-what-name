#ifndef _SPATIAL_HPP
#define _SPATIAL_HPP

#include <vector>

namespace JD::spatial
{
    void offsetsCreation();
    std::vector<std::pair<int, int>> calculateRegionsOffsets();
    void computeIndicies();

    void computeBlockIndicies();
}

#endif
