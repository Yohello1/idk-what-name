#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

#include <chlorine/logs/logs.hpp>
#include <chlorine/scene/scene.hpp>
#include <chlorine/scene/components.hpp>

namespace chlorine::io
{
    bool componentImport(::chlorine::scene::scene sceneIn, std::string filePath, ::chlorine::logging::logBase* logOut);
    bool sceneImport(::chlorine::scene::scene sceneIn, std::string filePath, ::chlorine::logging::logBase* logOut);
    ::chlorine::scene::component* objectFactory(std::string classname, std::string filePath);
}
