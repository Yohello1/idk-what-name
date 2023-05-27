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
#include "render/VBO.cpp"
#include "render/EBO.cpp"
#include "render/FBO.cpp"
#include "render/VAO.cpp"
#include "render/texture.cpp"
#include "render/shader.cpp"
#include "render/FrameBufferTex.cpp"
#include "render/FBshader.cpp"

std::atomic<bool> kys; // politely :3

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

    Shader inMyMind("render/shaders/vert.glsl", "render/shaders/frag.glsl");

    std::cout << "Shaders created (in theory)" << '\n';
    std::cout << "Arrays and whatnot being made\n";
    VAO vao;
    VBO vbo(vertices);
    EBO ebo(indices);

    vao.enableArrayIndex(vao, 0);
    vao.setArrayFormat(vao, 0, 3, 0);

    vao.enableArrayIndex(vao, 1);
    vao.setArrayFormat(vao, 1, 2, 3);

    vao.linkVBO(vao, 0, vbo, 0, 5*sizeof(GLfloat));
    vao.linkEBO(vao, ebo);

    std::cout << "Arrays and whatnot have been made\n";

    Texture map("render/test.png");

    std::cout << "Texture created" << '\n';

    FBO fbo;
    FrameBufferTex FrameBufferTex(fbo);
    fbo.fboWorking();
    FBShader fbshader("render/shaders/FBvert.glsl", "render/shaders/FBfrag.glsl");

    std::cout << "Framebuffer created \n";

    while(!kys)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo.ID);
        GLfloat backgroundColor[] = {255.0f/255.0f, 135.0f/255.0, 71.0f/255.0f,1.0f};
        glClearNamedFramebufferfv(fbo.ID, GL_COLOR, 0, backgroundColor);

        glUseProgram(inMyMind.shaderProgram);
        glBindTextureUnit(0, map.texture);
        glUniform1i(glGetUniformLocation(inMyMind.shaderProgram, "tex"), 0);
        glBindVertexArray(vao.ID);
        glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(indices[0]), GL_UNSIGNED_INT, 0);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glUseProgram(fbshader.FBShaderProgram);
        glBindTextureUnit(0, FrameBufferTex.ID);
        glUniform1i(glGetUniformLocation(fbshader.FBShaderProgram, "screen"), 0);
        glBindVertexArray(vao.ID);
        glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(indices[0]), GL_UNSIGNED_INT, 0);



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
