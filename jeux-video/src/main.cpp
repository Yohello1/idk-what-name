#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include <chlorine/logs/logs.hpp>
#include <chlorine/scene/scene.hpp>
#include <chlorine/scene/component.hpp>
#include <chlorine/io/io.hpp>

// std::shared_ptr<::chlorine::scene::component> ::chlorine::scene::createComponent1(std::unique_ptr<::chlorine::scene::scene> const& sceneIn, std::string filePath, std::map<std::string, std::shared_ptr<::chlorine::scene::component>> mapOfClasses, std::unique_ptr<::chlorine::logging::logBase> const &logOut)
// {

// }

class boxes : public ::chlorine::scene::component
{
public:
    std::pair<uint32_t, uint32_t> position;

    void dumpData(std::unique_ptr<::chlorine::logging::logBase> const &logOut) override
    {
        logOut->log("Pos:(" + std::to_string(position.first) + "," + std::to_string(position.second) + ")\n");
    }

    bool loadFile(std::fstream& componentFile, std::string filePath, std::unique_ptr<::chlorine::logging::logBase> const &logOut) override
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


    std::unique_ptr<chlorine::logging::logBase> logOut(new ::chlorine::logging::logToTerminal("LOG: ", "\x1B[32m"));

    logOut->log("hi\n");


    logOut->log("Atoms\n");


    std::unique_ptr<chlorine::scene::scene> tempScene = std::make_unique<::chlorine::scene::scene>(logOut);

    chlorine::io::sceneImport(tempScene, "../test/" ,"../test/Arrowhead.pcsf", mapOfClasses, logOut);

    logOut->log("hello");

    tempScene->components["boxes"]->dumpData(logOut);
}
