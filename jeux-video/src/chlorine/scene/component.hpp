#include <stdint.h>
#include <unordered_map>
#include <string>
#include <iostream>
#include <vector>

#include <chlorine/logs/logs.hpp>

#pragma once

namespace chlorine::scene
{
    // Note to anyone using these in the future:
    // All of these functions need to exist, and
    // BE PROPERLY DEFINED!!!
    class component
    {
    public:
        std::string componentName;
        std::unordered_map<std::string, uint16_t> locationMap;
    };

}
