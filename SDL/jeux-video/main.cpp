#include "include/stb/stb_image.h"
#include "include/stb/stb_truetype.h"
#include "start/base_data.hpp"
#include "generation/terain.hpp"
#include "start/update_funcs.hpp"
#include "physics/entity.hpp"
#include "physics/entity_types.hpp"
#include "debug/debug.hpp"
#include "render/render.hpp"
#include "render/VBO.cpp"
#include "render/EBO.cpp"
#include "render/VAO.cpp"
#include "render/texture.cpp"
#include "render/shader.cpp"
#include "render/Compute.cpp"
#include "render/MVPMatrix.hpp"
// #include "render/render.hpp"
#include "Game.cpp"


std::atomic<bool> kys; // politely :3

// ECS stuff, dont remove them, the system will kill itself
unsigned int currenttime = (unsigned int)time(NULL);
std::mutex mtx2;
entites::Coordinator Conductor;


GLfloat vertices[] =
{
	-64.0f, -64.0f , 0.0f, 0.3f, 0.0f,
	-64.0f,  64.0f , 0.0f, 0.3f, 1.0f,
	 64.0f,  64.0f , 0.0f, 1.0f, 1.0f,
	 64.0f, -64.0f , 0.0f, 1.0f, 0.0f,

	-64.0f, -64.0f , 5.0f, 0.0f, 0.0f,
	-64.0f,  64.0f , 5.0f, 0.0f, 1.0f,
	 64.0f,  64.0f , 5.0f, 1.0f, 1.0f,
	 64.0f, -64.0f , 5.0f, 1.0f, 0.0f,
};

GLuint indices[] =
{
	0, 2, 1,
	0, 3, 2,
	4, 6, 5,
	4, 7, 6
};


int main()
{
	// Honestly I have no idea whwre else to put this stuff
	srand(current_time);

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	unsigned int actual_width = 1024;
	unsigned int actual_height = 1024;
	unsigned int logical_width = 256;
	unsigned int logical_height = 256;

	GLFWwindow* window = glfwCreateWindow(actual_width, actual_height, "Gament2", NULL, NULL);
	if (!window)
	{
		std::cout << "Failed to create the GLFW window\n";
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(true);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize OpenGL context" << std::endl;
	}
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glViewport(0, 0, actual_width, actual_height);


	GLuint VAO, VBO, EBO;
	{
		glCreateVertexArrays(1, &VAO);
		glCreateBuffers(1, &VBO);
		glCreateBuffers(1, &EBO);

		glNamedBufferData(VBO, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glNamedBufferData(EBO, sizeof(indices), indices, GL_STATIC_DRAW);

		glEnableVertexArrayAttrib(VAO, 0);
		glVertexArrayAttribBinding(VAO, 0, 0);
		glVertexArrayAttribFormat(VAO, 0, 3, GL_FLOAT, GL_FALSE, 0);

		glEnableVertexArrayAttrib(VAO, 1);
		glVertexArrayAttribBinding(VAO, 1, 0);
		glVertexArrayAttribFormat(VAO, 1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat));

		glVertexArrayVertexBuffer(VAO, 0, VBO, 0, 5 * sizeof(GLfloat));
		glVertexArrayElementBuffer(VAO, EBO);
	}

	std::cout << "vert and frag shaders being made" << std::endl;
	Shader inMyMind("render/shaders/vert.glsl", "render/shaders/frag.glsl");
	std::cout << "vert and frag shaders have been made" << std::endl;
	inMyMind.useProgram();

	// Aspect, is well, the ratio of actual to logical
	float* fakeImg = new float[4194304];

	Game::Game scene1("hi", 1024, 1024, 4, 8, 1024, 1024, 1000);

	inMyMind.setMat4("uProjectionMatrix", scene1._transforms->ProjectionMatrix);
	inMyMind.setMat4("uViewMatrix",  scene1._transforms->ViewMatrix);
	inMyMind.setMat4("uModelMatrix",  scene1._transforms->ModelMatrix);

	Shaders::computeShader dejaVu("render/shaders/compute.glsl");
	// this shouldnt change
	Shaders::computeImageOut halfwayThroughNovember(1024,1024,0);
	Shaders::computeImageIn heyworld(1024,1024,1);
	Shaders::computeImageIn letItGo(1024,1024,2);              // Not used
	Shaders::computeImageIn cigaretteDayDreams(1024, 1024, 3); // Not used

	functions::fakeRandomImage(fakeImg, 0);
	heyworld.copyDataFloat(fakeImg);
	functions::fakeRandomImage(fakeImg, 0);
	letItGo.copyDataFloat(fakeImg);
	functions::fakeRandomImage(fakeImg, 0);
	cigaretteDayDreams.copyDataFloat(fakeImg);

	dejaVu.printMaxComputeSize();
	int work_grp_inv;
	glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &work_grp_inv);
	std::cout << "Max invocations count per work group: " << work_grp_inv << "\n";

	//fake image maker :)
	functions::fakeRandomImage(fakeImg, 0);
	scene1.updateImageBulk(0, fakeImg);
	std::cout << "IMG MADE2" << '\n';

	scene1.changeTitle(window);

	while (!glfwWindowShouldClose(window))
	{
		auto start_time = Clock::now();

		glClear(GL_COLOR_BUFFER_BIT);

		inMyMind.setMat4("uViewMatrix", scene1._transforms->ViewMatrix);
		scene1._transforms->rotateView(0.5,0.5,0.5);

		dejaVu.useProgram();
		glDispatchCompute(1024, 1024, 1);
		glMemoryBarrier(GL_ALL_BARRIER_BITS);

		scene1.updateImageBulk(0, fakeImg);
		std::copy(fakeImg, fakeImg + 1024*1024*4, scene1._layers.at(0));
		heyworld.copyDataFloat(scene1._layers.at(0));

		inMyMind.useProgram();
		glBindTextureUnit(0, halfwayThroughNovember.getID());
		glUniform1i(glGetUniformLocation(inMyMind.shaderProgram, "screen"), 0);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, 0);

		auto end_time = Clock::now();
		std::cout << "Delta time: " << std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count()/1000000 << '\n';

		float tempTime = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count()/1000000;
		tempTime /= 25;

		functions::fakeRandomImage(fakeImg, tempTime);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// ~scene1();
	free(fakeImg);

	std::cout << "HEllo world " << std::endl;

	glfwDestroyWindow(window);
	glfwTerminate();
}
