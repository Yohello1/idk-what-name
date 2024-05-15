#include <iostream>
#include <string>

#include <chlorine/logs/logs.hpp>
#include <chlorine/scene/scene.hpp>
#include <chlorine/scene/component.hpp>
#include <chlorine/io/io.hpp>

class boxes : public ::chlorine::scene::component
{
public:
    std::pair<uint32_t, uint32_t> position;
    void dumpData(::chlorine::logging::logBase* logOut) override
    {
        logOut->log("Pos:(" + std::to_string(position.first) + "," + std::to_string(position.second) + ")");
    }
};

int main()
{
    std::cout << "Hello world" << std::endl;

    chlorine::logging::logBase* logOut;
    chlorine::logging::logToTerminal helloWorld{"LOG: ", "\x1B[32m"};
    helloWorld.log("hi\n");
    logOut = &helloWorld;

    helloWorld.log("Atoms\n");


    chlorine::scene::scene tempScene(logOut);

    chlorine::io::sceneImport(tempScene, "../test/Arrowhead.pcsf", logOut);
}
