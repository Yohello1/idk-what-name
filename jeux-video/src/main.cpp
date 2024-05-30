#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cstdint>

#include <chlorine/logs/logs.hpp>
#include <chlorine/scene/scene.hpp>
#include <chlorine/io/io.hpp>
#include <chlorine/scene/componentManager.hpp>

class NameClass
{
public:
    std::string name;

    NameClass(std::string nameIn) : name(nameIn)
    {
        // idk
    }

};


int main()
{
    std::cout << "Hello world" << std::endl;

    std::unique_ptr<chlorine::logging::logBase> logOut(std::make_unique<chlorine::logging::logToTerminal>("LOG: ", "\x1B[32m"));

    std::unique_ptr<chlorine::scene::scene> tempScene = std::make_unique<::chlorine::scene::scene>(logOut);
    logOut->log("hello");

    // chlorine::io::sceneImport(tempScene, "../test/" ,"../test/Arrowhead.pcsf", logOut);

    chlorine::scene::orchestra<std::uint32_t, std::string, NameClass> Conductor;

    Conductor.insertElement<std::uint32_t, int>("hi",  32);
    Conductor.insertElement<std::string, std::string>("hi", "kac");
    Conductor.insertElement<NameClass, std::string>("hi", "kac");

    std::string a = Conductor.getElement<std::string>("hi");
    NameClass b = Conductor.getElement<NameClass>("hi");

    // int a = Conductor.getElement<int>("hi");
    // std::cout << a << std::endl;
    // struct Name b = Conductor.getElement<struct Name>("hi");
    // int c = Conductor.getElement<int>("hi", "uu");
    // Conductor.removeElement("hi", "uu");
    // int d = Conductor.getElement<int>("hi", "uu");

}
