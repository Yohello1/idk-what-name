#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <map>
#include <memory>
#include <functional>

#include <chlorine/logs/logs.hpp>
#include <chlorine/scene/scene.hpp>

namespace chlorine::io
{
    bool sceneImport(std::unique_ptr<::chlorine::scene::scene> const& sceneIn, std::string pathPrefix, std::string filePath, std::map<std::string, std::function<const std::type_info&(std::unique_ptr<::chlorine::scene::component>&)>> mapSwitcher, std::unique_ptr<::chlorine::logging::logBase> const &logOut);
}
