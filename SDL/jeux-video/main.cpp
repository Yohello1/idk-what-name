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
#include "render/Compute.cpp"
#include "render/MVPMatrix.hpp"

std::atomic<bool> kys; // politely :3

// ECS stuff, dont remove them, the system will kill itself
std::vector<ui::single_ui_element *> ui_elements;
unsigned int currenttime = (unsigned int)time(NULL);
std::mutex mtx2;
entites::Coordinator Conductor;


GLfloat vertices[] =
{
	-64.0f, -64.0f , 0.0f, 0.0f, 0.0f,
	-64.0f,  64.0f , 0.0f, 0.0f, 1.0f,
	 64.0f,  64.0f , 0.0f, 1.0f, 1.0f,
	 64.0f, -64.0f , 0.0f, 1.0f, 0.0f,
};

GLuint indices[] =
{
	0, 2, 1,
	0, 3, 2
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
	unsigned int logical_width = 128;
	unsigned int logical_height = 128;

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
	glUseProgram(inMyMind.shaderProgram);

	MVPMatrix::MVPMatrixes favoriteConvosInTheAM((ACTUAL_WINDOW_WIDTH/LOGICAL_WINDOW_HEIGH),1024,1024,1000);
	inMyMind.setMat4("uProjectionMatrix", favoriteConvosInTheAM.ProjectionMatrix);
	inMyMind.setMat4("uViewMatrix", favoriteConvosInTheAM.ViewMatrix);
	inMyMind.setMat4("uModelMatrix", favoriteConvosInTheAM.ModelMatrix);

	Shaders::computeShader dejaVu("render/shaders/compute.glsl");
	// this shouldnt change
	Shaders::computeImageOut halfwayThroughNovember(1024,1024,0);
	Shaders::computeImageIn heyworld(1024,1024,1);

	dejaVu.printMaxComputeSize();
	int work_grp_inv;
	glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &work_grp_inv);
	std::cout << "Max invocations count per work group: " << work_grp_inv << "\n";

	//fake image maker :)
	float* fakeImg = new float[4194304];
	functions::fakeRandomImage(fakeImg);
	std::cout << "IMG MADE2" << '\n';

	while (!glfwWindowShouldClose(window))
	{
		auto start_time = Clock::now();


		// Rotate the plane???
		{
			// float ang_x = 0.0, ang_y = 0.0, ang_z = 0.0;
			// // std::cout << ang_y << '\n';

			// glm::mat4 transformX = glm::rotate(glm::mat4(1.0f), glm::radians(ang_x), glm::vec3(1.0f, 0.0f, 0.0f));
			// glm::mat4 transformY = glm::rotate(glm::mat4(1.0f), glm::radians(ang_y), glm::vec3(0.0f, 1.0f, 0.0f));
			// glm::mat4 transformZ = glm::rotate(glm::mat4(1.0f), glm::radians(ang_z), glm::vec3(0.0f, 0.0f, 1.0f));

		   	// favoriteConvosInTheAM.ModelMatrix = transformX * transformY * transformZ * favoriteConvosInTheAM.ModelMatrix;
			// inMyMind.setMat4("uModelMatrix", favoriteConvosInTheAM.ModelMatrix);
		}

		// std::cout << "View Matrix location: " << inMyMind.getUniformID("uProjectionMatrix") << '\n';
		// favoriteConvosInTheAM.rotateView(1.0,0.0,0.0);
		inMyMind.setMat4("uViewMatrix", favoriteConvosInTheAM.ViewMatrix);

		dejaVu.useProgram();
		glDispatchCompute(1024, 1024, 1);
		glMemoryBarrier(GL_ALL_BARRIER_BITS);

		heyworld.copyDataFloat(fakeImg);
		// inMyMind.setMat4("uViewMatrix", viewMatrix);
		glUseProgram(inMyMind.shaderProgram);
		glBindTextureUnit(0, halfwayThroughNovember.getID());
		glUniform1i(glGetUniformLocation(inMyMind.shaderProgram, "screen"), 0);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, 0);

		auto end_time = Clock::now();
		//std::cout << "Delta time: " << std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count()/1000000 << '\n';

		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	glfwDestroyWindow(window);
	glfwTerminate();
}
