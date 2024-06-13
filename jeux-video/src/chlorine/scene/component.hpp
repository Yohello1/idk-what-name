#include <string>

#pragma once

namespace chlorine::scene
{
    // this class is LITERALLY a place holder
    class component
    {
    public:
        virtual void init() {} ;
        virtual void update() {};
        virtual bool loadFile(const std::string &filePath) {(void)filePath; return false;};
    };

}
