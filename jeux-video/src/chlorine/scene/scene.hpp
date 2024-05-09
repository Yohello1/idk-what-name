#include <stdint.h>
#include <unordered_map>
#include <string>
#include <iostream>

#include <chlorine/logs/logs.hpp>

#pragma once

namespace chlorine::scene
{
        class scene
        {
        // TODO: Switch it from vector to ECS system
        public:
                std::string sceneName;
                std::pair<uint32_t, uint32_t> windowSize;
                std::unordered_map<std::string, std::string> components;

                scene(::chlorine::logging::logBase* logOut);
        };
}
