#include "start/base_data.hpp"
#include "render/stb_truetype.h"
#include "math/vector_math.hpp"
#include "physics/physics2.hpp"
#include "render/render.hpp"
#include "generation/terain.hpp"
#include "render/ui.hpp"
#include "physics/entity.hpp"
#include "physics/entity_types.hpp"
#include "debug/debug.hpp"



// Simulated array everything is being fed
// cell pixels[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
// cell new_version[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
// Array responsble for the UI
struct colour user_interface[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
// USR input and changes, TODO SWITCH TO EQAUTION
cell usr_input[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
bool changed[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
cell render[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
void excecution_finished();
void draw_box(cord_2d start, cord_2d end);

std::vector<ui::single_ui_element *> ui_elements;
unsigned int currenttime = (unsigned int)time(NULL);
std::mutex mtx2;
entites::Coordinator Conductor;


// Vertices coordinates
GLfloat vertices[] =
{
	// -0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower left corner
	// 0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower right corner
	// 0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f, // Upper corner
	// -0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // Inner left
	// 0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // Inner right
	// 0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f // Inner down


    -1.0f,1.0f,0.0f, // Top left corner
    1.0f,-1.0f,0.0f, // right bottom corner
    1.0f,1.0f,0.0f, // top right corner

    -1.0f,-1.0f, 0.0f, // bottom left

};

// Indices for vertices order
GLuint indices[] =
{
	// 0, 3, 5, // Lower left triangle
	// 3, 2, 4, // Upper
	// 5, 4, 1 // Lower right triangle
    0,1,2,
    0,3,1
};



int main()
{
    std::cout << "Starting" << '\n';

    // Starting stuff
    srand(current_time);

    // AAAAAa
    // https://youtu.be/45MIykWJ-C4
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

    // Generates Shader object using shaders defualt.vert and default.frag
    Shader shaderProgram("render/shaders/test_vs.glsl", "render/shaders/test_fs.glsl");
	shaderProgram.Activate();
    float aspect = (float)ACTUAL_WINDOW_WIDTH/ACTUAL_WINDOW_HEIGH;
    float half_heigh = ACTUAL_WINDOW_HEIGH/2.f; //ortho size
    float half_width = half_heigh * aspect;
    shaderProgram.setMat4("uProjectionMatrix", glm::ortho(-half_width, half_width, -half_heigh, half_heigh, ortho_near, ortho_farr));
    glm::mat4 view_matrix(1);
    shaderProgram.setMat4("uViewMatrix", view_matrix);

    std::cout << "Shaders r not ready " << '\n';


    // Generates Vertex Array Object and binds it
    VAO VAO1;
    VAO1.Bind();

    // Generates Vertex Buffer Object and links it to vertices
    VBO VBO1(vertices, sizeof(vertices));
    // Generates Element Buffer Object and links it to indices
    // EBO EBO1(indices, sizeof(indices));
    EBO EBO1(indices, sizeof(vertices));


    // Links VBO to VAO
    VAO1.LinkVBO(VBO1, 0);
    // Unbind all to prevent accidentally modifying them
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    // array_clean_start(pixels);
    // std::thread physics(physics::simulate, pixels, new_version);

    while (!glfwWindowShouldClose(window))
    {
        glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);

 		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT);
		// Tell OpenGL which Shader Program we want to use
		// Bind the VAO so OpenGL knows to use it
		VAO1.Bind();
		// Draw primitives, number of indices, datatype of indices, index of indices
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
    }
    quit_now = true;
    // physics.join();
    std::cout << "ENDED" << '\n';
    excecution_finished();

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
