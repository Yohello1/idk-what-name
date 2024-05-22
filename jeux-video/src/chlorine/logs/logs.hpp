// ok I am going to try and figure out what exactly Im doing ToT
#pragma once

#include <iostream>
#include <string>
#include <fstream>

namespace chlorine::logging
{

    class logBase
    {
        public:
        virtual void log(std::string value);
        virtual ~logBase() = default;
    };

    class logToTerminal: public logBase
    {
        public:
            explicit logToTerminal(std::string prefix="", std::string colorCode="\\003[0m");
            void log(std::string value);
            virtual ~logToTerminal() = default;

        private:
            std::string _prefix;
            std::string _colorCode;
    };

    class logToFile: public logBase
    {
        public:
            explicit logToFile(std::string file, std::string prefix="");
            void log(std::string value);
            virtual ~logToFile();

        private:
            std::ofstream _filestream;
            std::string _file;
            std::string _prefix;
    };
}
