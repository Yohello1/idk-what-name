#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include <chlorine/logs/logs.hpp>
#include <chlorine/scene/scene.hpp>
#include <chlorine/scene/component.hpp>
#include <chlorine/io/io.hpp>

class boxes : public ::chlorine::scene::component
{
public:
    std::pair<uint32_t, uint32_t> position;

    void dumpData(::chlorine::logging::logBase* logOut) override
    {
        logOut->log("Pos:(" + std::to_string(position.first) + "," + std::to_string(position.second) + ")");
    }

    bool loadFile(std::fstream& componentFile, std::string filePath,  ::chlorine::logging::logBase* logOut) override
    {
        componentFile.clear();
        componentFile.seekg(0);

        std::string line;
        while(std::getline(componentFile, line))
        {
            std::istringstream iss(line);
            std::pair<std::string, std::string> tempPair;
            if(!(iss >> tempPair.first >> tempPair.second)) { break; }

            if(tempPair.first == "type")
                componentName = filePath;
            if(tempPair.first == "xpos")
                position.first = std::stoi(tempPair.second);
            if(tempPair.first == "ypos")
                position.second = std::stoi(tempPair.second);
        }

        return true;
    }
};

std::map<std::string, ::chlorine::scene::component*> mapOfClasses =
    {
        {"boxes", new boxes},
        {"component", new ::chlorine::scene::component}
    };

int main()
{
    std::cout << "Hello world" << std::endl;

    chlorine::logging::logBase* logOut;
    chlorine::logging::logToTerminal helloWorld{"LOG: ", "\x1B[32m"};
    helloWorld.log("hi\n");
    logOut = &helloWorld;

    helloWorld.log("Atoms\n");


    chlorine::scene::scene tempScene(logOut);

    chlorine::io::sceneImport(tempScene, "../test/Arrowhead.pcsf", mapOfClasses, logOut);
}
