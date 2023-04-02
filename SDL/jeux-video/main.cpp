#include "start/base_data.hpp"
#include "include/stb/stb_truetype.h"
#include "include/stb/stb_image.h"
#include "math/vector_math.hpp"
// #include "physics/physics2.hpp"
#include "render/render.hpp"
#include "generation/terain.hpp"
#include "render/ui.hpp"
#include "start/update_funcs.hpp"
#include "physics/entity.hpp"
#include "physics/entity_types.hpp"
#include "debug/debug.hpp"
#include "render/texture.hpp"

// Simulated array everything is being fed
cell_t pixels[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
// cell new_version[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
// Array responsble for the UI
// This stuff's apparently never defined LOL
// struct colour user_interface[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
// USR input and changes, TODO SWITCH TO EQAUTION
cell_t usr_input[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
bool changed[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
cell_t render[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
void excecution_finished();
void draw_box(cord_2d start, cord_2d end);

std::vector<ui::single_ui_element *> ui_elements;
unsigned int currenttime = (unsigned int)time(NULL);
std::mutex mtx2;
entites::Coordinator Conductor;

// texture data, move to class soon

// Vertices coordinates
/*
GLfloat vertices[] =
    {
        // -0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower left corner
        // 0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower right corner
        // 0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f, // Upper corner
        // -0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // Inner left
        // 0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // Inner right
        // 0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f // Inner down

        // X is x,
        // Y is Y
        // Z is the layer
        -1.0f, 1.0f, 0.0f,  // Top left corner
        1.0f, -1.0f, 0.0f,  // right bottom corner
        1.0f, 1.0f, 0.0f,   // top right corner
        -1.0f, -1.0f, 0.0f, // bottom left

};*/
GLfloat vertices[] =
    {
        // -0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower left corner
        // 0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower right corner
        // 0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f, // Upper corner
        // -0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // Inner left
        // 0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // Inner right
        // 0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f // Inner down

        // X is x,
        // Y is Y
        // Z is the layer
        /*-1.0f*(LOGICAL_WINDOW_WIDTH/4),  1.0f*(LOGICAL_WINDOW_HEIGH/4), 0.0f,  // Top left corner
         1.0f  *(LOGICAL_WINDOW_WIDTH/4), -1.0f*(LOGICAL_WINDOW_HEIGH/4), 0.0f,  // right bottom corner
         1.0f  *(LOGICAL_WINDOW_WIDTH/4),  1.0f*(LOGICAL_WINDOW_HEIGH/4), 0.0f,   // top right corner
        -1.0f  *(LOGICAL_WINDOW_WIDTH/4), -1.0f*(LOGICAL_WINDOW_HEIGH/4), 0.0f, // bottom left
    */
   // texture cords are apparently the vert cords
        // positions          // colors           // texture coords

        /**
         50.0f, 50.0f, 0.0f, 100.0f, 000.0f, 000.0f, 1000.0f, 100.0f,   // top right
         50.0f,-0.5f, 0.0f,  000.0f, 100.0f, 000.0f, 1000.0f, 000.0f,  // bottom right
        -50.0f,-0.5f, 0.0f,  000.0f, 000.0f, 100.0f, 0000.0f, 000.0f, // bottom left
        -50.0f, 0.5f, 0.0f,  100.0f, 100.0f, 000.0f, 0000.0f, 100.0f   // top left
*/
    // Coords                   Colours ig?           Texture coords
    0.0f,  0.0f,  0.0f,         0.0f,0.0f,0.0f,       0.0f,   0.0f,
    0.0f, -50.0f, 0.0f,         0.0f,0.0f,0.0f,       0.0f, -100000.0f,
   -50.0f, 0.0f,  0.0f,         0.0f,0.0f,0.0f,      -1.0f,  0.0f,
   -50.0f,-50.0f, 0.0f,         0.0f,0.0f,0.0f,      -10000.0f,-10000.0f

};

// Indices for vertices order
GLuint indices[] =
    {
        // 0, 3, 5, // Lower left triangle
        // 3, 2, 4, // Upper
        // 5, 4, 1 // Lower right triangle
        /*0, 1, 2,
        0, 3, 1*/
        0,3,2,
        0,1,3
};

int main()
{
    std::cout << "Starting" << '\n';

    // Starting stuff
    srand(current_time);

    // AAAAAa
    // https://youtu.be/45MIykWJ-C4
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    GLFWwindow *window = glfwCreateWindow(ACTUAL_WINDOW_WIDTH, ACTUAL_WINDOW_HEIGH, "idk", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Could not create window (GLFW)" << '\n';
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    gladLoadGL();
    glViewport(0, 0, ACTUAL_WINDOW_WIDTH, ACTUAL_WINDOW_HEIGH);
    std::cout << "WINDOW CREATED" << '\n';


    // Generates Shader object using shaders defualt.vert and default.frag
    Shader shaderProgram("render/shaders/test_vs.glsl", "render/shaders/test_fs.glsl");
    std::cout << "SHADERS CREATED" << '\n';
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);

    int widthImg, heightImg, numColch;
    unsigned char* texutre_data = stbi_load("render/test.png",&widthImg,&heightImg, &numColch, 0);

    GLuint texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // if there's an error here, I probs specified the wrong data type 
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, heightImg, numColch, 0, GL_RGBA, GL_UNSIGNED_BYTE, texutre_data);
    // glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(texutre_data);
    glBindTexture(GL_TEXTURE_2D, 0);

    GLuint tex0Uni = glGetUniformLocation(shaderProgram.ID, "tex0");
    shaderProgram.Activate();
    glUniform1i(tex0Uni, 0);


    std::cout << "GL STUFF DECLARED" << '\n';

    // GLuint texture = Textures::raw_texture_load("debug/rand.raw", 128, 128);
    //glBindTexture(GL_TEXTURE_2D, texture);

    std::cout << "TEXTURE CREATED" << '\n';

    float aspect = (float)ACTUAL_WINDOW_WIDTH / ACTUAL_WINDOW_HEIGH;
    float half_heigh = ACTUAL_WINDOW_HEIGH / 2.f; // ortho size
    float half_width = ACTUAL_WINDOW_WIDTH / 2.f;
    // half_heigh /= LOGICAL_WINDOW_HEIGH;
    half_width = half_width / (ACTUAL_WINDOW_HEIGH/LOGICAL_WINDOW_HEIGH);
    half_heigh = half_heigh / (ACTUAL_WINDOW_WIDTH/LOGICAL_WINDOW_WIDTH);

    std::cout << "HALVES CALCULATED" << '\n';

    // glOrthof(0.0f, ACTUAL_WINDOW_WIDTH, ACTUAL_WINDOW_HEIGH, 0.0f, 0.0f, 1.0f);
    shaderProgram.setMat4("uProjectionMatrix", glm::ortho(-half_width, half_width, -half_heigh, half_heigh, ortho_near, ortho_farr));

    std::cout << "PROJECTION MATRICIES DONE" << '\n';

    glm::mat4 view_matrix(1);
    shaderProgram.setMat4("uViewMatrix", view_matrix);
    glm::vec3 uColor_temp;
    std::cout << "VIEW MATRICIES DONE" << '\n';
    uColor_temp = glm::vec3(1.0f, 1.0f, 1.0f);
    // shaderProgram.setVec3("uColor_temp", uColor_temp);

    std::cout << "Shaders have been preped" << '\n';

    VAO VAO1;
    VAO1.Bind();
    VBO VBO1(vertices, sizeof(vertices));
    EBO EBO1(indices, sizeof(vertices));
    VAO1.LinkVBO(VBO1, 0);
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    std::cout << "EVERYTHING PREPPED" << '\n';
    // array_clean_start(pixels);
    // std::thread physics(physics::simulate, pixels, new_version);

    float temp_move_x = 0;

    while (!glfwWindowShouldClose(window))
    {
        temp_move_x += 0.001;
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        shaderProgram.Activate();
        glBindTexture(GL_TEXTURE_2D, texture);

        // Bind the VAO so OpenGL knows to use it
        VAO1.Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);
        view_matrix = glm::translate(view_matrix, glm::vec3(temp_move_x, 0, 0));
        shaderProgram.setMat4("uViewMatrix", view_matrix);

        // 

        glfwPollEvents();
    }
    quit_now = true;
    // physics.join();
    std::cout << "ENDED" << '\n';
    excecution_finished();
    glDeleteTextures(1, &texture);

    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    shaderProgram.Delete();
    glfwDestroyWindow(window);
    glfwTerminate();
}

void excecution_finished(void)
{

    // SDL_DestroyRenderer(renderer);
    // SDL_DestroyWindow(window);
    // SDL_Quit();
}

/**
 * @brief Literally just meant to throw random data whether it be colour or properties into the array
 *
 */
void make_temp_arrays()
{
    for (int i = 0; i < LOGICAL_WINDOW_HEIGH; i++)
    {
        for (int j = 0; j < LOGICAL_WINDOW_WIDTH; j++)
        {
            colour_t temp;

            std::random_device dev;
            std::mt19937 rng(dev());

            // Random colours
            temp.r = (rand() % 255);
            temp.g = (rand() % 255);
            temp.b = (rand() % 255);
            temp.a = (rand() % 255);

            pixels[i][j].set_col(temp);
        }
    }
}
