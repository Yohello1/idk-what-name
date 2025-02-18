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
