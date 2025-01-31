#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <map>
#include <memory>
#include <functional>
#include <typeindex>
#include <filesystem>
#include <any>
#include <chlorine/logs/logs.hpp>
#include <chlorine/scene/scene.hpp>

using chlorineObjectFactory = std::map<std::string, std::function<std::type_index(std::string, std::vector<std::any>, chlorine::scene::orchestra&)>>;

namespace chlorine::io
{
    extern std::map<std::type_index, std::string> _stringTypeConv;

    bool sceneImport(std::unique_ptr<::chlorine::scene::scene> const& sceneIn,
                     const std::string& pathPrefix,
                     const std::string& filePath,
                     const chlorineObjectFactory& mapSwitcher,
                     std::unique_ptr<::chlorine::logging::logBase> const& logOut);

    bool componentExport(std::unique_ptr<::chlorine::scene::scene> const& sceneIn,
                         const std::string& pathPrefix,
                         const std::string& filePath,
                         const chlorineObjectFactory& mapSwitcher,
                         std::unique_ptr<::chlorine::logging::logBase> const &logOut);

    bool sceneExport(std::unique_ptr<::chlorine::scene::scene> const& sceneIn,
                     std::string filePath,
                     std::unique_ptr<::chlorine::logging::logBase> const& logOut);

    bool componentImport(std::string componentPath, std::unique_ptr<::chlorine::scene::scene> const& sceneIn,
                         chlorineObjectFactory  mapSwitcher,
                         std::unique_ptr<::chlorine::logging::logBase> const &logOut);
}
