#include <chlorine/io/io.hpp>



namespace chlorine::io
{
    size_t splitStringToVector(const std::string &txt, std::vector<std::string> &strs, char ch)
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

    bool sceneImport(std::unique_ptr<::chlorine::scene::scene> const& sceneIn, std::string pathPrefix, std::string filePath, std::map<std::string, std::function<const std::type_info&(std::unique_ptr<::chlorine::scene::component>&)>> mapSwitcher, std::unique_ptr<::chlorine::logging::logBase> const &logOut)
    {
        // open the scene file
        std::fstream file;
        file.open(filePath, std::ios::in);
        if(file.is_open() == false)
        {
            logOut->log("Could not open file: " + filePath);
            return false;
        }

        // Every line is split up as follows:
        // [name of component/thingy] [value/path to component]
        std::string firstLine; // name of scene
        std::vector<std::string> splitFirstLine;

        std::getline(file, firstLine);
        splitStringToVector(firstLine, splitFirstLine, ' ');

        logOut->log("Strings: " + splitFirstLine[0] + splitFirstLine[1] + '\n');

        sceneIn->sceneName = splitFirstLine[1];

        // TODO: whilst still on scene data

        std::string nextLine;
        while(std::getline(file, nextLine))
        {
            std::vector<std::string> tempSplit;
            splitStringToVector(nextLine, tempSplit, ' ' );
            std::string componentPath = pathPrefix + sceneIn->sceneName + "/" + tempSplit[1];
            if(tempSplit[0] == "component")
            {
                // sceneIn->components.insert(std::make_pair(tempSplit[1], std::unique_ptr<::chlorine::scene::component>()));
                // logOut->log("Made component: " + tempSplit[1] + '\n');
                std::fstream componentFile;
                componentFile.open(componentPath, std::ios::in);
                if(file.is_open() == false)
                {
                    logOut->log("Cold not open component file: " + componentPath);
                }

                std::string componentFileLine, componentName;
                std::getline(componentFile, componentFileLine);
                std::vector<std::string> componentFileSplit;
                splitStringToVector(componentFileLine, componentFileSplit, ' '); // 0 = "type", 1 = the actual type name
                std::getline(componentFile, componentName);

                componentFile.close();

                std::unique_ptr<::chlorine::scene::component> tempPtr;
                const std::type_info& tempTypeInfo = mapSwitcher[componentFileSplit[1]](tempPtr);
                // Load file function
                sceneIn->Conductor.instruments[componentName].emplace(tempTypeInfo, std::move(tempPtr));
            }
            else if(tempSplit[0] == "xpos")
            {
                sceneIn->windowSize.first = std::stoi(tempSplit[1]);
            }
            else if(tempSplit[0] == "ypos")
            {
                sceneIn->windowSize.first = std::stoi(tempSplit[1]);
            }
        }

        file.close();

        return true;
    }
}
