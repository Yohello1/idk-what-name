#include <chlorine/io/io.hpp>

namespace chlorine::io
{
    bool sceneImport(::chlorine::scene::scene sceneIn, std::string filePath, ::chlorine::logging::logBase* logOut)
    {
        std::fstream file;
        file.open(filePath);

        if(file.is_open() == false)
        {
            logOut->log("Could not open file");
            return false;
        }

        // I dont know why but adding `::` to the start makes it works
        //
        ::std::string line;
        while(std::getline(file, line))
        {
            std::istringstream iss(line);
            std::string dataName, dataValue;
            if(!(iss >> dataName >> dataValue)) { break; }
            logOut->log(dataName);

        }

        // close file and stuff


        return true;
    }
}
