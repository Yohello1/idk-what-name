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
    // Just to access stuff
    // I literally copy pasted this code
    // God knows how it works
    template<typename... Ts>
    struct variantWrapper
    {
        std::variant<Ts...> & var;
        template<typename T>
        operator T() {return std::get<T>(var);}
    };

    template<typename... typesIn>
    using instrument = std::variant<typesIn...>;

    using instrumentId = decltype(std::declval<std::type_info>().hash_code());


    template<typename... typesIn>
    class orchestra
    {
        std::unordered_map<std::string, std::unordered_map<std::string, instrument<typesIn...>>> instruments;

        // honestly insert/set are the same thing :P
        // This will be fixed laterrr
        // switch to const char* instead, or hashes
        void insertElement(std::string key1, std::string key2, instrument<typesIn...> const& instrumentIn)
        {
            instruments[key1][key2] = instrumentIn;
        }
    }

}
