#include <stdint.h>
#include <unordered_map>
#include <string>
#include <iostream>
#include <vector>

#include <chlorine/logs/logs.hpp>
#include <chlorine/scene/component.hpp>

#pragma once

namespace chlorine::scene
{
    class scene
    {
            // TODO: Switch it from vector to ECS system
    public:
            std::string sceneName;
            std::pair<uint32_t, uint32_t> windowSize;
            std::unordered_map<std::string, ::chlorine::scene::component*> components;

            scene(::chlorine::logging::logBase* logOut);
    };
}
