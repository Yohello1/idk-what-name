// ok I am going to try and figure out what exactly Im doing ToT
#pragma once

#include <iostream>
#include <string>
#include <fstream>

namespace chlorine::logging
{
    class logToTerminal
    {
        public:
            explicit logToTerminal(std::string prefix="", std::string colorCode="\\003[0m");
            void log(std::string value);

        private:
            std::string _prefix;
            std::string _colorCode;
    };

    class logToFile
    {
        public:
            explicit logToFile(std::string file, std::string prefix="");
            void log(std::string value);
            void closeFile();

        private:
            std::ofstream _filestream;
            std::string _file;
            std::string _prefix;
    };
}
