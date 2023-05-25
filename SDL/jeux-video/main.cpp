#include "start/base_data.hpp"
#include "include/stb/stb_image.h"
#include "include/stb/stb_truetype.h"
#include "render/render.hpp"
#include "generation/terain.hpp"
#include "render/ui.hpp"
#include "start/update_funcs.hpp"
#include "physics/entity.hpp"
#include "physics/entity_types.hpp"
#include "debug/debug.hpp"
#include "render/VAO.cpp"
#include "render/VBO.cpp"
#include "render/EBO.cpp"


std::atomic<bool> kys; // politely :3
unsigned int sartedTime = (unsigned int)time(NULL);

// ECS stuff, dont remove them, the system will kill itself
std::vector<ui::single_ui_element *> ui_elements;
unsigned int currenttime = (unsigned int)time(NULL);
std::mutex mtx2;
entites::Coordinator Conductor;

int main()
{
    std::cout << "Hello World" << std::endl;

    // Creating the temp planes
    GLfloat vertices[]
    {
        // X,     Y,    Z,    U,    V
        -0.5f,-0.5f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
         0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
         0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
    };

    GLuint indices[] =
    {
        0,2,1,
        0,3,2
    };

    std::cout << " Verticies created?" << '\n';
    std::cout << "Creating window \n";

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(ACTUAL_WINDOW_WIDTH, ACTUAL_WINDOW_HEIGH, "Augustimo", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Window could not be created (GLFW) \n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    gladLoadGL();
    glViewport(0,0, ACTUAL_WINDOW_WIDTH, ACTUAL_WINDOW_HEIGH);
    std::cout << "Window has been created\n";

    // put the write the code related to
    // loading the shaders here
    // Shaders r weird

    while(!kys)
    {
        glfwSwapBuffers(window);
        glfwPollEvents();
        if(glfwWindowShouldClose(window))
        {
            kys = true;
        }
    }

    glfwDestroyWindow(window);
    glfwTerminate();


}
