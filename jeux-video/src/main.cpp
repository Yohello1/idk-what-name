#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include <chlorine/logs/logs.hpp>
#include <chlorine/scene/scene.hpp>
#include <chlorine/io/io.hpp>
#include <chlorine/scene/componentManager.hpp>

struct Position
{
    int x;
    int y;
} pos1, pos2, pos3;

struct Name
{
    std::string name;
} name1;


int main()
{
    std::cout << "Hello world" << std::endl;

    std::unique_ptr<chlorine::logging::logBase> logOut(std::make_unique<chlorine::logging::logToTerminal>("LOG: ", "\x1B[32m"));

    logOut->log("hi\n");


    logOut->log("Atoms\n");


    std::unique_ptr<chlorine::scene::scene> tempScene = std::make_unique<::chlorine::scene::scene>(logOut);
    logOut->log("hello");

    // chlorine::io::sceneImport(tempScene, "../test/" ,"../test/Arrowhead.pcsf", logOut);

    chlorine::scene::orchestra<int> Conductor;

    Conductor.insertElement("hi", "no", 32);
    Conductor.insertElement("hi", "ye", 73);
    Conductor.insertElement("hi", "uu", 98);

    int a = Conductor.getElement<int>("hi", "no");
    int b = Conductor.getElement<int>("hi", "ye");
    int c = Conductor.getElement<int>("hi", "uu");

    Conductor.removeElement("hi", "uu");

    int d = Conductor.getElement<int>("hi", "uu");

}
