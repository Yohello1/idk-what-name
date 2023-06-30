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


GLfloat vertices[] =
{
	-0.5f, -0.5f , 0.0f, 0.0f, 0.0f,
	-0.5f,  0.5f , 0.0f, 0.0f, 1.0f,
	 0.5f,  0.5f , 0.0f, 1.0f, 1.0f,
	 0.5f, -0.5f , 0.0f, 1.0f, 0.0f,
};

// {{{
GLuint indices[] =
{
	0, 2, 1,
	0, 3, 2
};

// }}}
int main()
{
	
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(ACTUAL_WINDOW_WIDTH, ACTUAL_WINDOW_HEIGH, "OpenGL Context", NULL, NULL);
	if (!window)
	{
		std::cout << "Failed to create the GLFW window\n";
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize OpenGL context" << std::endl;
	}
	glViewport(0, 0, ACTUAL_WINDOW_WIDTH, ACTUAL_WINDOW_HEIGH);
	glEnable(GL_BLEND);

	std::cout << "Viewport created\n";

	Shader inMyMind("render/shaders/vert.glsl", "render/shaders/frag.glsl");

	std::cout << "Shader created\n";


	// For some reason, setting it up with the classes kills it
	VAO vao;
	GLuint EBO, VBO;
	glCreateBuffers(1, &VBO);
	glCreateBuffers(1, &EBO);
	glNamedBufferData(VBO, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glNamedBufferData(EBO, sizeof(indices), indices, GL_STATIC_DRAW);
	vao.enableArrayIndex(vao, 0);
	vao.setArrayFormat(vao, 0, 3, 0);
	vao.enableArrayIndex(vao, 1);
	vao.setArrayFormat(vao, 1, 2, 3);
	glVertexArrayVertexBuffer(vao.ID, 0, VBO, 0, 5 * sizeof(GLfloat));
	glVertexArrayElementBuffer(vao.ID, EBO);

	std::cout << "Getting image \n";

	// Texture map("hamster2.png");

	std::cout << "Loaded image\n";


	// GLuint FBO;
	// glCreateFramebuffers(1, &FBO);
	FBO fbo;
	FrameBufferTex FrameBufferTex(fbo);
	GLuint FBshaderProgram;

	// TEMP
	// I should throw all of this into one big class files lmfao
	// The issue with scaling is def within these files but idk what the differences/issues are
	// Note: The issue is with the vert file
	// std::string FBvertSourceS = getFileContents("render/shaders/FBvert.glsl");
	// it's a scope, learn what it is
	{
		std::string FBfragSourceS = getFileContents("render/shaders/FBfrag.glsl");
		// std::string FBvertSourceS = getFileContents("render/shaders/FBvert.glsl");

		const char* FBfragSource = FBfragSourceS.c_str();
		// const char* FBvertSource = FBvertSourceS.c_str();

		GLuint FBVertShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(FBVertShader, 1, &FBvertSource, NULL);
		// glShaderSource(framebufferVertexShader, 1, &framebufferVertexShaderSource, NULL);
		glCompileShader(FBVertShader);
		GLuint FBFragShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource( FBFragShader, 1, &FBfragSource, NULL);
		glCompileShader( FBFragShader);

		FBshaderProgram = glCreateProgram();
		glAttachShader(FBshaderProgram, FBVertShader);
		glAttachShader(FBshaderProgram,  FBFragShader);
		glLinkProgram(FBshaderProgram);

		glDeleteShader(FBVertShader);
		glDeleteShader(FBFragShader);

		auto fboStatus = glCheckNamedFramebufferStatus(fbo.ID, GL_FRAMEBUFFER);
		if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer error: " << fboStatus << "\n";
	}


	// compute shader
	GLuint ComputeShader;
	{
		const char* ComputeShaderSource = R"(
		#version 460 core

		layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in
		layout(rgba32f, binding = 0) uniform image2D imgOutput;

		void main()
		{\n
			vec4 value = vec4(0.0,0.0,0.0,1.0);
			ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy);

			value.x = float(texelCoord.x)/(gl_NumWorkGroups.x);
			value.x = float(texelCoord.y)/(gl_NumWorkGroups.y);
		})";

		std::cout << ComputeShaderSource << '\n';

		GLuint ComputeShaderPart = glCreateShader(GL_COMPUTE_SHADER);
		glShaderSource( ComputeShaderPart, 1, &ComputeShaderSource, NULL);
		glCompileShader( ComputeShaderPart);
		Shaders::compileErrors(ComputeShaderPart, "COMPUTE");

		ComputeShader = glCreateProgram();
		glAttachShader(ComputeShader, ComputeShaderPart);
		glLinkProgram(ComputeShader);
		Shaders::compileErrors(ComputeShaderPart, "PROGRAM");
	}

	GLuint compInput; // rlly it's just an image
	const unsigned int textureWidthComp = 512, textureHeighComp = 512; // temp
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &compInput);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, compInput);
		glTextureStorage2D(compInput, 1, GL_RGBA32F, textureWidthComp, textureHeighComp);
		glTextureSubImage2D(compInput, 0, 0, 0, textureWidthComp, textureHeighComp, GL_RGBA, GL_FLOAT, NULL);
		glBindImageTexture(0, compInput, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
	}


	while (!glfwWindowShouldClose(window))
	{
		glBindFramebuffer(GL_FRAMEBUFFER, fbo.ID);
		GLfloat backgroundColor[] = { 19.0f / 255.0f, 34.0f / 255.0f, 44.0f / 255.0f, 1.0f };
		glClearNamedFramebufferfv(fbo.ID, GL_COLOR, 0, backgroundColor);

		glUseProgram(inMyMind.shaderProgram);
		glBindTextureUnit(0, compInput);
		glUniform1i(glGetUniformLocation(inMyMind.shaderProgram, "tex"), 0);
		glBindVertexArray(vao.ID);
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, 0);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glUseProgram(FBshaderProgram);
		glBindTextureUnit(0, FrameBufferTex.ID); // I geuinely thought this would attempt to create the objsect
		glUniform1i(glGetUniformLocation(FBshaderProgram, "screen"), 0);
		glBindVertexArray(vao.ID); // NO framebuffer VAO because I simply double the size of the rectangle to cover the whole screen
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, 0);

		glUseProgram(ComputeShader);
		// rlly it should be hooked to some constant for the map size but
		// i cannot be bothered
		glDispatchCompute((unsigned int) textureWidthComp, (unsigned int)textureHeighComp, 1);
		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	glfwDestroyWindow(window);
	glfwTerminate();
 }
