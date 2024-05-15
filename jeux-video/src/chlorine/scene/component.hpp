#include <stdint.h>
#include <unordered_map>
#include <string>
#include <iostream>
#include <vector>

#include <chlorine/logs/logs.hpp>

#pragma once

namespace chlorine::scene
{
    class component
    {
    public:
        std::string componentName;
        virtual void dumpData(::chlorine::logging::logBase* logOut) { logOut->log(componentName+"\n"); }
    };

}
