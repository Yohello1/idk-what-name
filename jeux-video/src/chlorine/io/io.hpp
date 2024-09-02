#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <map>
#include <memory>
#include <functional>
#include <typeindex>

#include <chlorine/logs/logs.hpp>
#include <chlorine/scene/scene.hpp>

namespace chlorine::io
{
    extern std::map<std::type_index, std::string> stringTypeConv;

    bool sceneImport(std::unique_ptr<::chlorine::scene::scene> const& sceneIn,
                     const std::string& pathPrefix,
                     const std::string& filePath,
                     const std::map<std::string, std::function<std::type_index(std::string, chlorine::scene::orchestra&)>>& mapSwitcher,
                     std::unique_ptr<::chlorine::logging::logBase> const& logOut);

    bool sceneExport(std::unique_ptr<::chlorine::scene::scene> const& sceneIn,
                     std::string filePath,
                     std::unique_ptr<::chlorine::logging::logBase> const& logOut);


}
