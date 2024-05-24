#include <stdint.h>
#include <unordered_map>
#include <string>
#include <iostream>
#include <vector>
#include <memory>

#include <chlorine/logs/logs.hpp>
#include <chlorine/scene/componentManager.hpp>

#pragma once

namespace chlorine::scene
{
    class scene
    {
        // TODO: Switch it from vector to ECS system
    public:
        std::string sceneName;
        std::pair<uint32_t, uint32_t> windowSize;


        scene(std::unique_ptr<::chlorine::logging::logBase> const &logOut);
    };
}
