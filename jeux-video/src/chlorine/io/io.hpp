#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

#include <chlorine/logs/logs.hpp>
#include <chlorine/scene/scene.hpp>

namespace chlorine::io
{
        bool sceneImport(::chlorine::scene::scene sceneIn, std::string filePath, ::chlorine::logging::logBase* logOut);
}
