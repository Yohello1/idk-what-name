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

// const unsigned int SCREEN_WIDTH = 720;
// const unsigned int SCREEN_HEIGHT = 720;

// const unsigned short OPENGL_MAJOR_VERSION = 4;
// const unsigned short OPENGL_MINOR_VERSION = 6;

// bool vSync = true;



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

GLuint indices[] =
{
	0, 2, 1,
	0, 3, 2
};

const char* framebufferVertexShaderSource = R"(#version 460 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 uvs;
out vec2 UVs;
void main()
{
	gl_Position = vec4(2.0 * pos.x, 2.0 * pos.y, 2.0 * pos.z, 1.000);
	UVs = uvs;
})";
const char* framebufferFragmentShaderSource = R"(#version 460 core
out vec4 FragColor;
uniform sampler2D screen;
in vec2 UVs;
void main()
{
	FragColor = texture(screen, UVs);
})";


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

	Texture map("render/test.png");

	std::cout << "Loaded image\n";

	FBO fbo;
	FrameBufferTex FrameBufferTex(fbo);
	fbo.fboWorking();
	FBShader fbshader("render/shaders/FBvert.glsl", "render/shaders/FBfrag.glsl");


	// GLuint FBO;
	// glCreateFramebuffers(1, &FBO);

	// GLuint framebufferTex;
	// glCreateTextures(GL_TEXTURE_2D, 1, &framebufferTex);
	// glTextureParameteri(framebufferTex, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	// glTextureParameteri(framebufferTex, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// glTextureParameteri(framebufferTex, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	// glTextureParameteri(framebufferTex, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// glTextureStorage2D(framebufferTex, 1, GL_RGB8, ACTUAL_WINDOW_WIDTH, ACTUAL_WINDOW_HEIGH);
	// glNamedFramebufferTexture(FBO, GL_COLOR_ATTACHMENT0, framebufferTex, 0);

	// auto fboStatus = glCheckNamedFramebufferStatus(FBO, GL_FRAMEBUFFER);
	// if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
	// 	std::cout << "Framebuffer error: " << fboStatus << "\n";

	// GLuint framebufferVertexShader = glCreateShader(GL_VERTEX_SHADER);
	// glShaderSource(framebufferVertexShader, 1, &framebufferVertexShaderSource, NULL);
	// glCompileShader(framebufferVertexShader);
	// GLuint framebufferFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// glShaderSource(framebufferFragmentShader, 1, &framebufferFragmentShaderSource, NULL);
	// glCompileShader(framebufferFragmentShader);

	// GLuint framebufferShaderProgram = glCreateProgram();
	// glAttachShader(framebufferShaderProgram, framebufferVertexShader);
	// glAttachShader(framebufferShaderProgram, framebufferFragmentShader);
	// glLinkProgram(framebufferShaderProgram);

	// glDeleteShader(framebufferVertexShader);
	// glDeleteShader(framebufferFragmentShader);


	while (!glfwWindowShouldClose(window))
	{
		glBindFramebuffer(GL_FRAMEBUFFER, fbo.ID);
		GLfloat backgroundColor[] = { 19.0f / 255.0f, 34.0f / 255.0f, 44.0f / 255.0f, 1.0f };
		glClearNamedFramebufferfv(fbo.ID, GL_COLOR, 0, backgroundColor);

		glUseProgram(inMyMind.shaderProgram);
		glBindTextureUnit(0, map.texture1);
		glUniform1i(glGetUniformLocation(inMyMind.shaderProgram, "tex"), 0);
		glBindVertexArray(vao.ID);
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, 0);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glUseProgram(fbshader.ID);
		glBindTextureUnit(0, FrameBufferTex.ID);
		glUniform1i(glGetUniformLocation(fbshader.ID, "screen"), 0);
		glBindVertexArray(vao.ID); // NO framebuffer VAO because I simply double the size of the rectangle to cover the whole screen
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	glfwDestroyWindow(window);
	glfwTerminate();
}
