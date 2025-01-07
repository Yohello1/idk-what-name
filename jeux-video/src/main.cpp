#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cstdint>
#include <functional>
#include <typeindex>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <chlorine/logs/logs.hpp>
#include <chlorine/utils/strings.hpp>
#include <chlorine/scene/scene.hpp>
#include <chlorine/io/io.hpp>
#include <chlorine/scene/component.hpp>
#include <chlorine/scene/componentManager.hpp>

void endItAll()
{
    // kill everything I think?
}


class boxes : public chlorine::scene::component
{
public:
    std::pair<std::uint32_t, std::uint32_t> position;

    bool loadFile (const std::string &filePath, std::unique_ptr<::chlorine::logging::logBase> const &logOut) override
    {
        (void)logOut;


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
                std::vector<std::string> temppString;
                chlorine::utils::splitStringToVector(line, temppString, ' ');
                position.first = std::stoi(temppString[1]);
            }
            else if(lineNumber == 4)
            {
                std::vector<std::string> temppString;
                chlorine::utils::splitStringToVector(line, temppString, ' ');
                position.first = std::stoi(temppString[1]);
            }
        }

        return true;
    }

    bool saveFile(const std::string &filePath, std::unique_ptr<::chlorine::logging::logBase> const &logOut) override
    {
        (void)logOut;
        std::ofstream componentFile;
        componentFile.open(filePath);
        componentFile << "type " << chlorine::io::_stringTypeConv[std::type_index(typeid(*this))] << '\n';
        componentFile << name << '\n';
        componentFile << "xpos " << position.first << '\n';
        componentFile << "ypos " << position.second << '\n';
        componentFile.close();
        return true;
    }


	virtual ~boxes() = default;
};

class window_container : public chlorine::scene::component
{
public:
    GLFWwindow *window;

    uint16_t width, height;

    window_container(std::string string_in)
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

        window = glfwCreateWindow(width, height, "draw2", NULL, NULL);
        if (!window)
        {
            glfwTerminate();
            exit(EXIT_FAILURE);
        }

        std::vector<std::string> splitVector;

        ::chlorine::utils::splitStringToVector(string_in, &splitVector, ' ');

    }

    void update()
    {
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);



        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (key == GLFW_KEY_Q && action == GLFW_PRESS)
            endItAll();
    }

    bool loadFile (const std::string &filePath, std::unique_ptr<::chlorine::logging::logBase> const &logOut) override
    {
        (void)logOut;

        return true;
    }

    bool saveFile(const std::string &filePath, std::unique_ptr<::chlorine::logging::logBase> const &logOut) override
    {
        (void)logOut;

        return true;
    }


	virtual ~window_container() = default;
};


std::map<std::string, std::function<std::type_index(std::string, chlorine::scene::orchestra&)>> tempMap3
{
    {"boxes", [](std::string stringIn, chlorine::scene::orchestra& Conductor) -> std::type_index{
        Conductor.instruments[stringIn].emplace(std::type_index(typeid(boxes)), std::make_unique<boxes>());
        return std::type_index(typeid(boxes));}
    },
    {"window_container", [](std::string stringIn, chlorine::scene::orchestra& Conductor) -> std::type_index{
        Conductor.instruments[stringIn].emplace(std::type_index(typeid(window_container)),
                                                std::make_unique<window_container>());
        return std::type_index(typeid(window_container));}
    }


};


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
