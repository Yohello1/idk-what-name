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
#include <set>


#include <chlorine/logs/logs.hpp>
#include <chlorine/scene/component.hpp>
namespace chlorine::scene
{
    class orchestra
    {
    public:
        std::unordered_map<std::string, std::unordered_map<std::type_index, std::unique_ptr<::chlorine::scene::component>>> instruments;

        template<typename T, typename ...Args>
        void insertElement(std::string key1, std::unique_ptr<::chlorine::logging::logBase> const &logOut, Args ...args)
        {
            std::unique_ptr<::chlorine::scene::component> tempComponent = std::make_unique<T>(args...);
            instruments[key1].emplace(std::type_index(typeid(T)), std::make_unique<T>(args...));
            _groups[std::type_index(typeid(T))].insert(key1);


            logOut->log("Added element" + key1 + " " + std::type_index(typeid(T)).name() + '\n');
        }

        // Removes an element??? defined in the cpp file
        void removeElement(std::string key1, std::type_index key2, std::unique_ptr<::chlorine::logging::logBase> const &logOut);

        // add everything to their respective groups
        // Incredibly expensive, only use when speed is NOT a concern
        // Defined in the cpp file
        void groupAllElements(std::unique_ptr<::chlorine::logging::logBase> const &logOut);

        // get number of enenties of certain time
        std::size_t getElementTypeNum(std::type_index type);

    private:
        std::unordered_map<std::type_index, std::set<std::string>> _groups; // I do NOT want this to be modified directly
                                                                            // make flat_set at some point?
    };

}

// this file, and the io.cpp file are neck and neck for the most cancerous
// This is no longer true
