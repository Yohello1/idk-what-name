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
        ::std::string line;

        std::vector<std::pair<std::string, std::string>> tempData;

        while(std::getline(file, line))
        {
            std::istringstream iss(line);
            std::pair<std::string, std::string> tempPair;

            if(!(iss >> tempPair.first >> tempPair.second)) { break; }
            logOut->log("Data looks like: " + tempPair.first + " " + tempPair.second + "\n");

            tempData.push_back(tempPair);
        }

        // note: spee ddoesnt matter here :P
        for(int i = 0; i < tempData.size(); i++)
        {
            if(tempData[i].first == "name:")
                sceneIn.sceneName = tempData[i].second;
        }

        logOut->log(""+sceneIn.sceneName);

        // close file and stuff


        return true;
    }
}
