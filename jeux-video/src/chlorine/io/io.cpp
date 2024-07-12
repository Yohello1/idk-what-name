#include <chlorine/io/io.hpp>
#include <chlorine/utils/strings.hpp>


namespace chlorine::io
{
    bool componentImport(std::string componentPath, std::unique_ptr<::chlorine::scene::scene> const& sceneIn, std::map<std::string, std::function<std::type_index(std::string, chlorine::scene::orchestra&)>>  mapSwitcher, std::unique_ptr<::chlorine::logging::logBase> const &logOut)
    {
        std::fstream componentFile;
        componentFile.open(componentPath, std::ios::in);
        if(componentFile.is_open() == false)
        {
             logOut->log("Could not open file" + componentPath);
             return false;
        }

        std::string componentFileLine, componentName;
        std::getline(componentFile, componentFileLine);
        std::vector<std::string> componentFileSplit;
        ::chlorine::utils::splitStringToVector(componentFileLine, componentFileSplit, ' ');
        std::getline(componentFile, componentName);

        componentFile.close();

        std::type_index tempIndex =
            mapSwitcher[componentFileSplit[1]](componentFileSplit[1], sceneIn->Conductor);
        sceneIn->Conductor.instruments[componentFileSplit[1]][tempIndex]->loadFile(componentPath);

        return true;
    }

    bool sceneImport(std::unique_ptr<::chlorine::scene::scene> const& sceneIn, std::string pathPrefix, std::string filePath, std::map<std::string, std::function<std::type_index(std::string, chlorine::scene::orchestra&)>>  mapSwitcher, std::unique_ptr<::chlorine::logging::logBase> const &logOut)
    {
        // open the scene file
        std::fstream sceneFile;
        sceneFile.open(filePath, std::ios::in);
        if(sceneFile.is_open() == false)
        {
            logOut->log("Could not open file: " + filePath);
            return false;
        }

        // Every line is split up as follows:
        // [name of component/thingy] [value/path to component]
        std::string firstLine; // name of scene
        std::vector<std::string> splitFirstLine;
        std::getline(sceneFile, firstLine);
        ::chlorine::utils::splitStringToVector(firstLine, splitFirstLine, ' ');
        sceneIn->sceneName = splitFirstLine[1];

        // TODO: whilst still on scene data


        std::string nextLine;
        while(std::getline(sceneFile, nextLine))
        {
            std::vector<std::string> tempSplit;
            ::chlorine::utils::splitStringToVector(nextLine, tempSplit, ' ' );
            if(tempSplit[0] == "component")
            {
                std::string componentPath = pathPrefix + sceneIn->sceneName + "/" + tempSplit[1];
                componentImport(componentPath, sceneIn, mapSwitcher, logOut);
            }
        }

        sceneFile.close();

        sceneIn->Conductor.groupAllElements(logOut);

        return true;
    }
}
