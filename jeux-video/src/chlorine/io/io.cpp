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

    bool sceneImport(std::unique_ptr<::chlorine::scene::scene> const& sceneIn, std::string pathPrefix, std::string filePath, std::unique_ptr<::chlorine::logging::logBase> const &logOut)
    {
        std::fstream file;
        file.open(filePath, std::ios::in);

        if(file.is_open() == false)
        {
            logOut->log("Could not open file: " + filePath);
            return false;
        }

        // Every line is split up as follows:
        // [name of component/thingy] [value/path to component]
        std::string firstLine;
        std::string nextLine;
        std::vector<std::string> splitFirstLine;

        std::getline(file, firstLine);
        splitStringToVector(firstLine, splitFirstLine, ' ');

        logOut->log("Strings: " + splitFirstLine[0] + splitFirstLine[1] + '\n');

        sceneIn->sceneName = splitFirstLine[1];

        while(std::getline(file, nextLine))
        {
            std::vector<std::string> tempSplit;
            splitStringToVector(nextLine, tempSplit, ' ' );
            std::string componentPath = pathPrefix + sceneIn->sceneName + "/" + tempSplit[1];
            if(tempSplit[0] == "component")
            {
                // sceneIn->components.insert(std::make_pair(tempSplit[1], std::unique_ptr<::chlorine::scene::component>()));
                // logOut->log("Made component: " + tempSplit[1] + '\n');
            }
        }

        return true;
    }
}
