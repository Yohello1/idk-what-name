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
        void insertElement(std::string key1, Args ...args)
        {
            std::unique_ptr<::chlorine::scene::component> tempComponent = std::make_unique<T>(args...);
            instruments[key1].emplace(typeid(T), std::move(tempComponent));
        }

        void removeElement(std::string key1, std::type_index key2);

        // add everything to their respective groups
        void groupAllElements();

    private:
        std::unordered_map<std::type_index, std::vector<std::string>> _groups; // I do NOT want this to be modified directly
    };

}

// this file, and the io.cpp file are neck and neck for the most cancerous
