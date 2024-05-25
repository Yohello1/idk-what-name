#pragma once

#include <string>
#include <unordered_map>
#include <map>
#include <iostream>
#include <vector>
#include <typeinfo>
#include <sstream>
#include <cstdint>
#include <variant>
#include <utility>
#include <typeindex>
#include <typeinfo>
#include <memory>

#include <chlorine/logs/logs.hpp>

namespace chlorine::scene
{
    template<typename... typesIn>
    class orchestrator
    {
    public:
        std::unordered_map<std::string, std::unordered_map<std::string, std::variant<std::monostate, typesIn...>>> tempMap;

        template<typename typeIn>
        bool addStructToMap(typeIn valueIn, std::string key1, std::string key2)
        {
            tempMap<typeIn>[key1][key2] = valueIn;
            return true;
        }
    private:
    };
}
