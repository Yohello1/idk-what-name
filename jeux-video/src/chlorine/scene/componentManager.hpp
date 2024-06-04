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
#include <chlorine/scene/component.hpp>
namespace chlorine::scene
{
    class orchestra
    {
    public:
        std::unordered_map<std::string, std::unordered_map<std::type_index, std::unique_ptr<::chlorine::scene::component>>> instruments;

        template<typename T, typename ...Args>
        void insertElement(std::string key1, Args&&... args)
        {
            // instruments[key1].emplace(typeid(T), std::unique_ptr<std::uint32_t>(new std::uint32_t));
            // instruments[key1][typeid(T)] = std::unique_ptr<std::uint32_t>(new std::uint32_t(12));
            // instruments[key1][typeid(T)] = std::make_unique<std::uint32_t>(12);
            // instruments[key1].insert(std::make_pair(typeid(T), std::unique_ptr<T>(new T(std::forward<Args>(args)...))));
            instruments[key1][typeid(T)] = std::unique_ptr<T>();
        }

        // find of hash next
        void removeElement(std::string key1, std::type_index key2);

    private:
        std::unordered_map<std::type_index, std::vector<std::string>> _groups; // I do NOT want this to be modified directly
    };

}
