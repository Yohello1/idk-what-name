#include <chlorine/io/io.hpp>
#include <chlorine/utils/strings.hpp>


namespace chlorine::io
{
    std::map<std::type_index, std::string> _stringTypeConv;

    bool componentImport(std::string componentPath, std::unique_ptr<::chlorine::scene::scene> const& sceneIn,
                         std::map<std::string,
                         std::function<std::type_index(std::string, chlorine::scene::orchestra&)>>  mapSwitcher,
                         std::unique_ptr<::chlorine::logging::logBase> const &logOut)
    {


        std::fstream componentFile;
        componentFile.open(componentPath, std::ios::in);
        if(componentFile.is_open() == false)
        {
            logOut->log("Could not open file" + componentPath + "\n");
            return false;
        }

        std::string componentFileLine, componentName;
        std::getline(componentFile, componentFileLine);
        std::vector<std::string> componentFileSplit;
        ::chlorine::utils::splitStringToVector(componentFileLine, componentFileSplit, ' ');
        std::getline(componentFile, componentName);

        componentFile.close();

        if(mapSwitcher.find(componentFileSplit[1]) == mapSwitcher.end())
        {
            logOut->log("Could not find function to create the component\n");
            return false;
        }

        std::type_index tempIndex = mapSwitcher[componentFileSplit[1]](componentFileSplit[1], sceneIn->Conductor);
        sceneIn->Conductor.instruments[componentFileSplit[1]][tempIndex]->loadFile(componentPath, logOut);

        // If the type_index does not exist in the map, add it to the map

        _stringTypeConv.emplace(tempIndex, componentFileSplit[1]);
        logOut->log(_stringTypeConv[tempIndex] + "\n");

        return true;
    }

    bool sceneImport(std::unique_ptr<::chlorine::scene::scene> const& sceneIn,
                     const std::string& pathPrefix,
                     const std::string& filePath,
                     const std::map<std::string, std::function<std::type_index(std::string, chlorine::scene::orchestra&)>>& mapSwitcher,
                     std::unique_ptr<::chlorine::logging::logBase> const& logOut)
    {
        // open the scene file
        std::fstream sceneFile;
        sceneFile.open(filePath, std::ios::in);
        if(sceneFile.is_open() == false)
        {
            logOut->log("Could not open file: " + filePath + "\n");
            return false;
        }

        // Every line is split up as follows:
        // [name of component] [path to component]
        std::string firstLine; // name of scene
        std::vector<std::string> splitFirstLine;
        std::getline(sceneFile, firstLine);
        ::chlorine::utils::splitStringToVector(firstLine, splitFirstLine, ' ');
        sceneIn->sceneName = splitFirstLine[1];

        // Since everything is a component of the Scene, we dont really need this.
        // First part being the name of the coomponent

        std::string nextLine;
        while(std::getline(sceneFile, nextLine))
        {
            std::vector<std::string> tempSplit;
            ::chlorine::utils::splitStringToVector(nextLine, tempSplit, ' ' );

            std::string componentPath = pathPrefix + sceneIn->sceneName + "/" + tempSplit[0] + ".pccf";
            logOut->log(componentPath + '\n');
            logOut->log(nextLine + '\n');
            if(!componentImport(componentPath, sceneIn, mapSwitcher, logOut))
            {
                logOut->log("Unable to open file, giving up: " + componentPath + "\n");
                return false;
            }

        }
        sceneFile.close();
        sceneIn->Conductor.groupAllElements(logOut);

        return true;
    }

    // bool componentImport(std::string componentPath, std::unique_ptr<::chlorine::scene::scene> const& sceneIn,
    //                      std::map<std::string,
    //                      std::function<std::type_index(std::string, chlorine::scene::orchestra&)>>  mapSwitcher,
    //                      std::unique_ptr<::chlorine::logging::logBase> const &logOut)
    // {

    // }

    bool sceneExport(std::unique_ptr<::chlorine::scene::scene> const& sceneIn,
                     std::string filePath,
                     // std::map<std::type_index, std::string> mapConv,
                     std::unique_ptr<::chlorine::logging::logBase> const& logOut)
    {
        // TODO
        // Ok write Scene name first line
        // Component name file path


        _stringTypeConv.size();
        for(auto const& [key, value] : sceneIn->Conductor.instruments )
        {
            logOut->log("key: " + key + "For this item name or wtv\n");
            for(auto const& [component, va] : value)
            {
                logOut->log("val: " + _stringTypeConv[component] + "\n");
            }
        }

        filePath += sceneIn->sceneName + ".pcsf";

        // First we make the pcsf file;
        std::ofstream sceneFile;
        sceneFile.open(filePath);

        sceneFile << "name " << sceneIn->sceneName << '\n';
        logOut->log("name " + sceneIn->sceneName + '\n');


        sceneFile.close();

        return false;
    }

}
