#include <chlorine/io/io.hpp>

namespace chlorine::io
{

    size_t split(const std::string &txt, std::vector<std::string> &strs, char ch)
    {
        size_t pos = txt.find( ch );
        size_t initialPos = 0;
        strs.clear();

        // Decompose statement
        while( pos != std::string::npos ) {
            strs.push_back( txt.substr( initialPos, pos - initialPos ) );
            initialPos = pos + 1;

            pos = txt.find( ch, initialPos );
        }

        // Add the last one
        strs.push_back( txt.substr( initialPos, std::min( pos, txt.size() ) - initialPos + 1 ) );

        return strs.size();
    }

    // what the cinamon toast fuck is this
    ::chlorine::scene::component* createComponent(std::fstream& file, std::string componentType, std::string filePath, std::map<std::string, ::chlorine::scene::component*> mapOfClasses,std::unique_ptr<::chlorine::logging::logBase> const &logOut)
    {
        auto desiredComponent = mapOfClasses.find(componentType)->second;
        desiredComponent->loadFile(file, filePath, logOut);
        desiredComponent->dumpData(logOut);
        return desiredComponent;
    }

    bool componentImport(std::unique_ptr<::chlorine::scene::scene> const& sceneIn, std::string filePath, std::map<std::string, ::chlorine::scene::component*> mapOfClasses, std::unique_ptr<::chlorine::logging::logBase> const &logOut)
    {
        std::fstream file;
        file.open(filePath);

        if(file.is_open() == false)
        {
            logOut->log("Could not open file: " + filePath);
            return false;
        }

        logOut->log("Successfully opened: " + filePath + '\n');

        std::string firstLine;
        std::vector<std::string> listParts;
        std::getline(file, firstLine);
        ::chlorine::io::split(firstLine, listParts, ' ');
        logOut->log(listParts[0]);

        logOut->log("Napped" + '\n');

        if(listParts[0] != "type")
            return false;

        logOut->log("creating component");
        ::chlorine::scene::component* temp = createComponent(file, listParts[1], filePath, mapOfClasses, logOut);
        // std::unique_ptr<::chlorine::scene::component> tempUnique(temp);

        return true;

    }

    bool sceneImport(std::unique_ptr<::chlorine::scene::scene> const& sceneIn, std::string pathPrefix, std::string filePath, std::map<std::string,  ::chlorine::scene::component*> mapOfClasses, std::unique_ptr<::chlorine::logging::logBase> const &logOut)
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

        logOut->log("Sucessfully opened & Loaded scene: " + sceneIn->sceneName + "\n");

        return true;
    }
}
