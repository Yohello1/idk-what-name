#pragma once

#include <string>
#include <vector>

namespace chlorine::utils
{
        size_t splitStringToVector(const std::string &txt, std::vector<std::string> &strs, char ch);
}
