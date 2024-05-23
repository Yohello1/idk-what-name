#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include <chlorine/logs/logs.hpp>
#include <chlorine/scene/scene.hpp>
#include <chlorine/io/io.hpp>
#include <chlorine/scene/componentManager.hpp>

int main()
{
    std::cout << "Hello world" << std::endl;

    std::unique_ptr<chlorine::logging::logBase> logOut(std::make_unique<chlorine::logging::logToTerminal>("LOG: ", "\x1B[32m"));

    logOut->log("hi\n");


    logOut->log("Atoms\n");


    std::unique_ptr<chlorine::scene::scene> tempScene = std::make_unique<::chlorine::scene::scene>(logOut);

    chlorine::io::sceneImport(tempScene, "../test/" ,"../test/Arrowhead.pcsf", logOut);

    std::string boxesss = "boxes";
    std::string patthh = "../test/Arrowhead/boxes.pccf";


    logOut->log("hello");

    // logOut.reset();
    // temp.reset();


}
