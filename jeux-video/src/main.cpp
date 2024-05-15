#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <memory>

#include <chlorine/logs/logs.hpp>
#include <chlorine/scene/scene.hpp>
#include <chlorine/scene/component.hpp>
#include <chlorine/io/io.hpp>

class boxes : public ::chlorine::scene::component
{
public:
    std::pair<uint32_t, uint32_t> position;

    void dumpData(std::unique_ptr<::chlorine::logging::logBase> const &logOut) override
    {
        logOut->log("Pos:(" + std::to_string(position.first) + "," + std::to_string(position.second) + ")");
    }

    bool loadFile(std::fstream& componentFile, std::string filePath,  std::unique_ptr<::chlorine::logging::logBase> const &logOut) override
    {
        logOut->log("noo");
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

std::map<std::string, std::unique_ptr<::chlorine::scene::component>> mapOfClasses;

int main()
{
    std::cout << "Hello world" << std::endl;


    std::unique_ptr<chlorine::logging::logBase> logOut(new ::chlorine::logging::logToTerminal("LOG: ", "\x1B[32m"));

    logOut->log("hi\n");

    mapOfClasses.insert(std::make_pair("boxes", std::unique_ptr<boxes>(new boxes)));
    mapOfClasses.insert(std::make_pair("component", std::unique_ptr<::chlorine::scene::component>(new ::chlorine::scene::component)));

    logOut->log("Atoms\n");


    std::unique_ptr<chlorine::scene::scene> tempScene(new ::chlorine::scene::scene(logOut));

    // chlorine::io::sceneImport(tempScene, "../test/" ,"../test/Arrowhead.pcsf", mapOfClasses, logOut);
}
