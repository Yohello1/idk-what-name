#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <map>
#include <memory>

#include <chlorine/logs/logs.hpp>
#include <chlorine/scene/scene.hpp>
#include <chlorine/scene/component.hpp>

namespace chlorine::io
{
    bool componentImport(::chlorine::scene::scene& sceneIn, std::map<std::string, ::chlorine::scene::component*>& mapOfClasses, std::string filePath, ::chlorine::logging::logBase* logOut);
    bool sceneImport(::chlorine::scene::scene& sceneIn, std::string pathPrefix, std::string filePath,   std::map<std::string, ::chlorine::scene::component*>& mapOfClasses, ::chlorine::logging::logBase* logOut);
}
