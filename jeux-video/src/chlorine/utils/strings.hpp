#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <locale>
#include <cstring>

namespace chlorine::utils
{
    std::size_t splitStringToVector(const std::string &txt, std::vector<std::string> &strs, char ch);
    std::size_t stripString(const std::string &str, const std::string &subSt);
}
