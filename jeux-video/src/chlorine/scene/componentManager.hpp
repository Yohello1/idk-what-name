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
#include <memory>
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
    public:
        std::unordered_map<std::string, std::unordered_map<std::type_index, instrument<typesIn...>>> instruments;

        // honestly insert/set are the same thing :P
        // This will be fixed laterrr
        // switch to const char* instead, or hashes
        template<typename T, typename... Args>
        void insertElement(std::string key1, Args&&... args)
        {
            // std::type_index key2 = typeid(T);
            // instruments[key1][key2] = instrumentIn;
            instruments[key1].emplace(typeid(T), std::forward<T>(args)...);
        }

        template<typename T>
        T getElement(std::string key1)
        {
            std::type_index key2 = typeid(T);
            return variantWrapper{instruments[key1][key2]};
        }

        // find of hash next
        void removeElement(std::string key1, std::type_index key2)
        {
            instruments[key1].erase(key2);
        }


        // std::vector<std::string> getByInstrument(std::vector<std::string> wantedInstruments)
        // {
        //     if(wantedInstruments.size() == 0)
        //     {
        //         return {};
        //     }

        //     std:vector<std::string> result;
        //     if(auto const found = instruments.find(components[0]); found == end(instruments))
        //     {
        //         return {};
        //     }
        //     else
        //     {
        //         result = {};// fuck if i know
        //     }
        // }
    private:
        std::unordered_map<std::type_index, std::vector<std::string>> _groups;
    };

}
