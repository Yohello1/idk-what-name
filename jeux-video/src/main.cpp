#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cstdint>
#include <functional>

#include <chlorine/logs/logs.hpp>
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


};

std::map<std::string, std::function<void(std::unique_ptr<chlorine::scene::component>&)>> tempMap{
    {"char", [](std::unique_ptr<chlorine::scene::component>& myPointer){ myPointer.reset(new numClass()); } }
};


int main()
{
    std::cout << "Hello world" << std::endl;

    std::unique_ptr<chlorine::logging::logBase> logOut(std::make_unique<chlorine::logging::logToTerminal>("LOG: ", "\x1B[32m"));

    std::unique_ptr<chlorine::scene::scene> tempScene = std::make_unique<::chlorine::scene::scene>(logOut);
    logOut->log("hello");

    // chlorine::io::sceneImport(tempScene, "../test/" ,"../test/Arrowhead.pcsf", logOut);

    chlorine::scene::orchestra Conductor;

    Conductor.insertElement<chlorine::scene::component>("hi");
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
