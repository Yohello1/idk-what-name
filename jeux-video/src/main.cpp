#include <iostream>
#include <string>

#include <chlorine/logs/logs.hpp>
#include <chlorine/scene/scene.hpp>
#include <chlorine/io/io.hpp>

int main()
{
    std::cout << "Hello world" << std::endl;

    chlorine::logging::logBase* logOut;
    chlorine::logging::logToTerminal helloWorld{"LOG: ", "\x1B[32m"};
    helloWorld.log("hi\n");
    logOut = &helloWorld;

    helloWorld.log("Atoms\n");


    chlorine::scene::scene tempScene(logOut);

    chlorine::io::sceneImport(tempScene, "tempData.txt", logOut);
}
