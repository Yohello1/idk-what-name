#include "io.hpp"

namespace chlorine::io
{
    bool sceneImport(::chlorine::scene::scene sceneIn, std::string filePath)
    {
        std::fstream file;
        file.open(filePath);

        if(file.is_open() == false)
            return false;

        std:string line;
        while(std::getline(file, line))
        {
            std::istringstream iss(line);
            std::string dataName, dataValue;
            if(!(iss >> dataName >> dataValue)) { break; }
            // data processing
        }

        // close file and stuff


        return true;
    }
}
