#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cstdint>
#include <functional>

#include <chlorine/logs/logs.hpp>
//#include <chlorine/utils/strings.cpp>
#include <chlorine/scene/scene.hpp>
#include <chlorine/io/io.hpp>
#include <chlorine/scene/component.hpp>
#include <chlorine/scene/componentManager.hpp>

class NameClass : public chlorine::scene::component
{
public:
    std::string name;

    NameClass(std::string const& nameIn) : name(nameIn)
    {
        // idk
    }

    void changeName(std::string nameIn)
    {
        name = nameIn;
    }

};

class numClass : public chlorine::scene::component
{
public:
    int num;
    numClass()
    {
        num = rand();
    }

    numClass(int x)
    {
        num = x;
    }
};

class boxes : public chlorine::scene::component
{
public:
    std::pair<std::uint32_t, std::uint32_t> position;
    std::string name;

    bool loadFile (const std::string &filePath) override
    {
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

                position.first = 4;

            }
            else if(lineNumber == 4)
                position.second = 8;
        }

        return true;
    }

};

std::map<std::string, std::function<const std::type_info&(std::unique_ptr<chlorine::scene::component>&)>> tempMap{
    {"boxes", [](std::unique_ptr<chlorine::scene::component>& myPointer)-> const std::type_info&{myPointer.reset(new boxes()); return typeid(boxes);}}
};


int main()
{
    std::cout << "Hello world" << std::endl;

    std::unique_ptr<chlorine::logging::logBase> logOut(std::make_unique<chlorine::logging::logToTerminal>("LOG: ", "\x1B[32m"));

    std::unique_ptr<chlorine::scene::scene> tempScene = std::make_unique<::chlorine::scene::scene>(logOut);
    logOut->log("hello");

    chlorine::io::sceneImport(tempScene, "../test/" ,"../test/Arrowhead.pcsf", tempMap, logOut);

    chlorine::scene::orchestra Conductor;

    // Conductor.insertElement<chlorine::scene::component>("hi");
    // Conductor.insertElement<numClass, int>("hi", 32);

    // chlorine::scene::component* tempBase = Conductor.instruments["hi"][typeid(numClass)].get();
    // numClass* tempDerived = static_cast<numClass*>(tempBase);
    // int a = tempDerived->num;
   // logOut->log(tempDerived->num);

    // int a = Conductor.getElement<int>("hi");
    // std::cout << a << std::endl;
    // struct Name b = Conductor.getElement<struct Name>("hi");
    // int c = Conductor.getElement<int>("hi", "uu");
    // Conductor.removeElement("hi", "uu");
    // int d = Conductor.getElement<int>("hi", "uu");

}
