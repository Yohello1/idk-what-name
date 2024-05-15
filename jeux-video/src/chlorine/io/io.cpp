#include <chlorine/io/io.hpp>

namespace chlorine::io
{

    bool componentImport(std::unique_ptr<::chlorine::scene::scene> const& sceneIn, std::string filePath, std::map<std::string, std::shared_ptr<::chlorine::scene::component>> mapOfClasses, std::unique_ptr<::chlorine::logging::logBase> const &logOut)
    {
        std::fstream file;
        file.open(filePath);
        logOut->log("Opened file: " + filePath + '\n');

        if(file.is_open() == false)
        {
            logOut->log("Could not open file: " + filePath);
            return false;
        }

        logOut->log("Successfully opened: " + filePath + '\n');

        std::string firstLine;
        std::getline(file, firstLine);
        std::istringstream iss(firstLine);
        std::pair<std::string, std::string> tempPair;
        iss >> tempPair.first >> tempPair.second;
        // we assume first line is type
        logOut->log("Read first line" + tempPair.first);
        if(tempPair.first != "type")
            return false;

        logOut->log("wa\n");

        auto desiredComponent = mapOfClasses.find(tempPair.first)->second;
        // if(desiredComponent == mapOfClasses.end())
        //     return false;
        logOut->log("path\n");

        desiredComponent->loadFile(file, filePath, logOut);
        logOut->log("qqq\n");
        desiredComponent->dumpData(logOut);

        return true;

    }

    bool sceneImport(std::unique_ptr<::chlorine::scene::scene> const& sceneIn, std::string pathPrefix, std::string filePath, std::map<std::string, std::shared_ptr<::chlorine::scene::component>> mapOfClasses, std::unique_ptr<::chlorine::logging::logBase> const &logOut)
    {
        std::fstream file;
        file.open(filePath, std::ios::in);

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

        // note: speed doesnt matter here :P
        for(int i = 0; i < tempData.size(); i++)
        {
            if(tempData[i].first == "name")
                sceneIn->sceneName = tempData[i].second;
        }

        // note: speed doesnt matter here :P
        for(int i = 0; i < tempData.size(); i++)
        {
            if(tempData[i].first == "component")
            {
                std::string componentPath = pathPrefix + sceneIn->sceneName + "/" + tempData[i].second;
                logOut->log("Path to open " + componentPath + '\n');
                componentImport(sceneIn, componentPath, mapOfClasses, logOut);
            }

        }

        logOut->log(""+sceneIn->sceneName);

        // close file and stuff


        return true;
    }
}