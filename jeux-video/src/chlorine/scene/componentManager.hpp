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
    template<typename... T>
    class orchestrator
    {
    public:
        std::unordered_map<std::string, std::unordered_map<std::size_t, std::variant<std::monostate, T...>>> entities;
        std::unordered_map<std::type_index, std::size_t> noodles;

        template<typename compT> // component Template
        bool setValue(std::string key, compT value, std::unique_ptr<::chlorine::logging::logBase> const &logOut);

    private:
    };
}
