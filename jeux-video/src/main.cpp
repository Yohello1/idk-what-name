#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cstdint>

#include <chlorine/logs/logs.hpp>
#include <chlorine/scene/scene.hpp>
#include <chlorine/io/io.hpp>
#include <chlorine/scene/component.hpp>
#include <chlorine/scene/componentManager.hpp>

class NameClass
{
public:
    std::string name;

    NameClass(std::string nameIn) : name(nameIn)
    {
        // idk
    }

    void changeName(std::string nameIn)
    {
        name = nameIn;
    }

};


int main()
{
    std::cout << "Hello world" << std::endl;

    std::unique_ptr<chlorine::logging::logBase> logOut(std::make_unique<chlorine::logging::logToTerminal>("LOG: ", "\x1B[32m"));

    std::unique_ptr<chlorine::scene::scene> tempScene = std::make_unique<::chlorine::scene::scene>(logOut);
    logOut->log("hello");

    // chlorine::io::sceneImport(tempScene, "../test/" ,"../test/Arrowhead.pcsf", logOut);

    chlorine::scene::orchestra Conductor;

    Conductor.insertElement<std::uint32_t, int>("hi",  32);


    // int a = Conductor.getElement<int>("hi");
    // std::cout << a << std::endl;
    // struct Name b = Conductor.getElement<struct Name>("hi");
    // int c = Conductor.getElement<int>("hi", "uu");
    // Conductor.removeElement("hi", "uu");
    // int d = Conductor.getElement<int>("hi", "uu");

}
