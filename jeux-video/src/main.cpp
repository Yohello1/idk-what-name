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

    chlorine::scene::orchestrator tempConductor;

    pos1.x = 10;
    pos1.y = 20;

    tempConductor.setValue("bob", pos1, logOut);


}
