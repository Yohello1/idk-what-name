#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cstdint>
#include <functional>
#include <typeindex>
#include <any>

#include <chlorine/logs/logs.hpp>
#include <chlorine/utils/strings.hpp>
#include <chlorine/scene/scene.hpp>
#include <chlorine/io/io.hpp>
#include <chlorine/scene/component.hpp>
#include <chlorine/scene/componentManager.hpp>


class boxes : public chlorine::scene::component
{
public:
    std::pair<std::uint32_t, std::uint32_t> position;

    bool loadFile (const std::string &filePath, std::unique_ptr<::chlorine::logging::logBase> const &logOut) override
    {
        (void)logOut;


        std::fstream componentFile;
        componentFile.open(filePath);

        std::string line;
        std::uint32_t lineNumber = 0;

        while(std::getline(componentFile, line))
        {
            lineNumber++;
            if(lineNumber == 2)
                name = line;
            else if(lineNumber == 3)
            {
                std::vector<std::string> temppString;
                chlorine::utils::splitStringToVector(line, temppString, ' ');
                position.first = std::stoi(temppString[1]);
            }
            else if(lineNumber == 4)
            {
                std::vector<std::string> temppString;
                chlorine::utils::splitStringToVector(line, temppString, ' ');
                position.first = std::stoi(temppString[1]);
            }
        }

        return true;
    }

    bool saveFile(const std::string &filePath, std::unique_ptr<::chlorine::logging::logBase> const &logOut) override
    {
        (void)logOut;
        std::ofstream componentFile;
        componentFile.open(filePath);
        componentFile << "type " << chlorine::io::_stringTypeConv[std::type_index(typeid(*this))] << '\n';
        componentFile << name << '\n';
        componentFile << "xpos " << position.first << '\n';
        componentFile << "ypos " << position.second << '\n';
        componentFile.close();
        return true;
    }


	virtual ~boxes() = default;
};

class windowX : public chlorine::scene::component
{
public:
    std::uint16_t windowX;
    bool loadFile(const std::string &filePath, std::unique_ptr<::chlorine::logging::logBase> const &logOut) override
    {
        (void)logOut;
        std::fstream componentFile;
        componentFile.open(filePath);

        std::string line;
        std::uint32_t lineNumber = 0;

        while(std::getline(componentFile, line))
        {
            lineNumber++;
            if(lineNumber == 2)
            {
                name = line;
            }
            else if(lineNumber == 3)
            {
                std::vector<std::string> tempString;
                chlorine::utils::splitStringToVector(line, tempString, ' ');
                windowX = std::stoi(tempString[1]);
            }
        }
        return true;
    }

    bool saveFile(const std::string &filePath, std::unique_ptr<::chlorine::logging::logBase> const &logOut) override
    {
        (void)logOut;
        std::ofstream componentFile;
        componentFile.open(filePath);
        componentFile << "type " << chlorine::io::_stringTypeConv[std::type_index(typeid(*this))] << '\n';
        componentFile << name << '\n';
        componentFile << name << ' ' << windowX << '\n';
        componentFile.close();
        return true;
    }
};

class windowY : public chlorine::scene::component
{
public:
    std::uint16_t windowY;
    bool loadFile(const std::string &filePath, std::unique_ptr<::chlorine::logging::logBase> const &logOut) override
    {
        std::fstream componentFile;
        componentFile.open(filePath);

        std::string line;
        std::uint32_t lineNumber = 0;

        while(std::getline(componentFile, line))
        {
            lineNumber++;
            if(lineNumber == 2)
            {
                name = line;
            }
            else if(lineNumber == 3)
            {
                std::vector<std::string> tempString;
                chlorine::utils::splitStringToVector(line, tempString, ' ');
                windowY = std::stoi(tempString[1]);
            }
        }
        return true;
    }

    bool saveFile(const std::string &filePath, std::unique_ptr<::chlorine::logging::logBase> const &logOut) override
    {
        (void)logOut;
        std::ofstream componentFile;
        componentFile.open(filePath);
        componentFile << "type " << chlorine::io::_stringTypeConv[std::type_index(typeid(*this))] << '\n';
        componentFile << name << '\n';
        componentFile << name << ' ' << windowY << '\n';
        componentFile.close();
        return true;
    }
};

std::map<std::string, std::function<std::type_index(std::string, std::vector<std::any>,  chlorine::scene::orchestra&)>> tempMap3
{
    {"boxes", [](std::string stringIn, std::vector<std::any> initVals, chlorine::scene::orchestra& Conductor) -> std::type_index{
        Conductor.instruments[stringIn].emplace(std::type_index(typeid(boxes)), std::make_unique<boxes>());
        return std::type_index(typeid(boxes));}
    },
    {"windowX", [](std::string stringIn, std::vector<std::any> initVals, chlorine::scene::orchestra& Conductor) -> std::type_index{
        Conductor.instruments[stringIn].emplace(std::type_index(typeid(windowX)), std::make_unique<windowX>());
        return std::type_index(typeid(windowX));}
    },
    {"windowY", [](std::string stringIn, std::vector<std::any> initVals, chlorine::scene::orchestra& Conductor) -> std::type_index{
        Conductor.instruments[stringIn].emplace(std::type_index(typeid(windowY)), std::make_unique<windowY>());
        return std::type_index(typeid(windowY));}
    },
};


int main()
{
    std::cout << "Hello world" << std::endl;

    std::unique_ptr<chlorine::logging::logBase> logOut(std::make_unique<chlorine::logging::logToTerminal>("LOG: ", "\x1B[32m"));

    std::unique_ptr<chlorine::scene::scene> tempScene = std::make_unique<::chlorine::scene::scene>(logOut);
    logOut->log("hello\n");

    chlorine::io::sceneImport(tempScene, "../test/" ,"../test/timeSpent.pcsf", tempMap3, logOut);
    // tempScene->sceneName = "timeSpent";
    // chlorine::io::sceneExport(tempScene, "../test/", logOut);

}
