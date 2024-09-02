#include <string>

#include <chlorine/logs/logs.hpp>

#pragma once

namespace chlorine::scene
{
    // this class is LITERALLY a place holder
    class component
    {
    public:
        std::string name;

        virtual void init() {} ;
        virtual void update() {};
        virtual bool loadFile(const std::string &filePath, std::unique_ptr<::chlorine::logging::logBase> const &logOut) {(void)filePath; (void)logOut; return false;};
    virtual bool saveFile(const std::string &filePath, std::unique_ptr<::chlorine::logging::logBase> const &logOut) {(void)filePath; (void)logOut; return false;};
        virtual ~component() { }
    };

}
