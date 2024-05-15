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
    bool componentImport(std::unique_ptr<::chlorine::scene::scene> const& sceneIn, std::string filePath, std::map<std::string, std::shared_ptr<::chlorine::scene::component>> mapOfClasses, std::unique_ptr<::chlorine::logging::logBase> const &logOut);
    bool sceneImport(std::unique_ptr<::chlorine::scene::scene> const& sceneIn, std::string pathPrefix, std::string filePath,   std::map<std::string, ::chlorine::scene::component*>& mapOfClasses, std::unique_ptr<::chlorine::logging::logBase> const &logOu);
}
