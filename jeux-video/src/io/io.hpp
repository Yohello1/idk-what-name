#pragma once

#include <fstream>
#include <sstream>
#include <string>

namespace chlorine::io
{
        bool sceneImport(::chlorine::scene::scene sceneIn, std::string filePath);
}
