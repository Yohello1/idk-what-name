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

//
GLuint indices[] =
{
	0, 2, 1,
	0, 3, 2
};

//
int main()
{

	
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	GLFWwindow* window = glfwCreateWindow(ACTUAL_WINDOW_WIDTH, ACTUAL_WINDOW_HEIGH, "OpenGL Context", NULL, NULL);
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
	glViewport(0, 0, ACTUAL_WINDOW_WIDTH, ACTUAL_WINDOW_HEIGH);
	// glEnable(GL_BLEND);

	std::cout << "Viewport created\n";

	//Shader inMyMind("render/shaders/vert.glsl", "render/shaders/frag.glsl");

	std::cout << "Shader created\n";


	// For some reason, setting it up with the classes kills it
	// VAO vao;

	GLuint EBO, VBO, vaowo;
	glCreateBuffers(1, &VBO);
	glCreateBuffers(1, &EBO);
	glNamedBufferData(VBO, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glNamedBufferData(EBO, sizeof(indices), indices, GL_STATIC_DRAW);
	// vao.enableArrayIndex(vao, 0);
	// vao.setArrayFormat(vao, 0, 3, 0);
	// vao.enableArrayIndex(vao, 1);
	// vao.setArrayFormat(vao, 1, 2, 3);
	glEnableVertexArrayAttrib(vaowo, 0);
	glVertexArrayAttribBinding(vaowo, 0, 0);
	glVertexArrayAttribFormat(vaowo, 0, 3, GL_FLOAT, GL_FALSE, 0);

	glEnableVertexArrayAttrib(vaowo, 1);
	glVertexArrayAttribBinding(vaowo, 1, 0);
	glVertexArrayAttribFormat(vaowo, 1, 2, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat));

	glVertexArrayVertexBuffer(vaowo, 0, VBO, 0, 5 * sizeof(GLfloat));
	glVertexArrayElementBuffer(vaowo, EBO);

	// std::cout << "Getting image \n";
	// Texture map("hamster2.png");
	// std::cout << "Loaded image\n";


	// GLuint FBO;
	// glCreateFramebuffers(1, &FBO);
	// FBO fbo;
	// FrameBufferTex FrameBufferTex(fbo);
	// GLuint FBshaderProgram;

	// TEMP
	// I should throw all of this into one big class files lmfao
	// The issue with scaling is def within these files but idk what the differences/issues are
	// Note: The issue is with the vert file
	// std::string FBvertSourceS = getFileContents("render/shaders/FBvert.glsl");
	// it's a scope, learn what it is
	/*
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
	*/

	// Compute Shader

	const char* screenVertexShaderSource = R"(#version 460 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 uvs;
out vec2 UVs;
void main()
{
	gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);
	UVs = uvs;
})";
const char* screenFragmentShaderSource = R"(#version 460 core
out vec4 FragColor;
uniform sampler2D screen;
in vec2 UVs;
void main()
{
	FragColor = texture(screen, UVs);
})";
const char* screenComputeShaderSource = R"(#version 460 core
layout(local_size_x = 8, local_size_y = 4, local_size_z = 1) in;
layout(rgba32f, binding = 0) uniform image2D screen;
void main()
{
	vec4 pixel = vec4(0.075, 0.133, 0.173, 1.0);
	ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.xy);

	ivec2 dims = imageSize(screen);
	float x = -(float(pixel_coords.x * 2 - dims.x) / dims.x); // transforms to [-1.0, 1.0]
	float y = -(float(pixel_coords.y * 2 - dims.y) / dims.y); // transforms to [-1.0, 1.0]

	float fov = 90.0;
	vec3 cam_o = vec3(0.0, 0.0, -tan(fov / 2.0));
	vec3 ray_o = vec3(x, y, 0.0);
	vec3 ray_d = normalize(ray_o - cam_o);

	vec3 sphere_c = vec3(0.0, 0.0, -5.0);
	float sphere_r = 1.0;

	vec3 o_c = ray_o - sphere_c;
	float b = dot(ray_d, o_c);
	float c = dot(o_c, o_c) - sphere_r * sphere_r;
	float intersectionState = b * b - c;
	vec3 intersection = ray_o + ray_d * (-b + sqrt(b * b - c));

	if (intersectionState >= 0.0)
	{
		pixel = vec4((normalize(intersection - sphere_c) + 1.0) / 2.0, 1.0);
	}

	imageStore(screen, pixel_coords, pixel);
})";


	GLuint screenTex;
	glCreateTextures(GL_TEXTURE_2D, 1, &screenTex);
	glTextureParameteri(screenTex, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(screenTex, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTextureParameteri(screenTex, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(screenTex, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTextureStorage2D(screenTex, 1, GL_RGBA32F, 512, 512);
	glBindImageTexture(0, screenTex, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

	GLuint screenVertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(screenVertexShader, 1, &screenVertexShaderSource, NULL);
	glCompileShader(screenVertexShader);
	GLuint screenFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(screenFragmentShader, 1, &screenFragmentShaderSource, NULL);
	glCompileShader(screenFragmentShader);

	GLuint screenShaderProgram = glCreateProgram();
	glAttachShader(screenShaderProgram, screenVertexShader);
	glAttachShader(screenShaderProgram, screenFragmentShader);
	glLinkProgram(screenShaderProgram);

	glDeleteShader(screenVertexShader);
	glDeleteShader(screenFragmentShader);


	GLuint computeShader = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(computeShader, 1, &screenComputeShaderSource, NULL);
	glCompileShader(computeShader);

	GLuint computeProgram = glCreateProgram();
	glAttachShader(computeProgram, computeShader);
	glLinkProgram(computeProgram);


	int work_grp_cnt[3];
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &work_grp_cnt[0]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &work_grp_cnt[1]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &work_grp_cnt[2]);
	std::cout << "Max work groups per compute shader" <<
		" x:" << work_grp_cnt[0] <<
		" y:" << work_grp_cnt[1] <<
		" z:" << work_grp_cnt[2] << "\n";

	int work_grp_size[3];
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &work_grp_size[0]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &work_grp_size[1]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &work_grp_size[2]);
	std::cout << "Max work group sizes" <<
		" x:" << work_grp_size[0] <<
		" y:" << work_grp_size[1] <<
		" z:" << work_grp_size[2] << "\n";

	int work_grp_inv;
	glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &work_grp_inv);
	std::cout << "Max invocations count per work group: " << work_grp_inv << "\n";


	while (!glfwWindowShouldClose(window))
	{
		// // glBindFramebuffer(GL_FRAMEBUFFER, fbo.ID);
		// GLfloat backgroundColor[] = { 19.0f / 255.0f, 34.0f / 255.0f, 44.0f / 255.0f, 1.0f };
		// // glClearNamedFramebufferfv(fbo.ID, GL_COLOR, 0, backgroundColor);



		// glUseProgram(inMyMind.shaderProgram);
		// glBindTextureUnit(0, screenTex);
		// glUniform1i(glGetUniformLocation(inMyMind.shaderProgram, "screen"), 0);
		// glBindVertexArray(vaowo);
		// glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, 0);
		// // glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// // glUseProgram(FBshaderProgram);
		// // glBindTextureUnit(0, FrameBufferTex.ID); // I geuinely thought this would attempt to create the objsect
		// // glUniform1i(glGetUniformLocation(FBshaderProgram, "screen"), 0);
		// // glBindVertexArray(vao.ID); // NO framebuffer VAO because I simply double the size of the rectangle to cover the whole screen
		// // glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, 0);

		// glfwSwapBuffers(window);
		// glfwPollEvents();

		glUseProgram(computeProgram);
		glDispatchCompute(128, 256, 1);
		glMemoryBarrier(GL_ALL_BARRIER_BITS);

		glUseProgram(screenShaderProgram);
		glBindTextureUnit(0, screenTex);
		glUniform1i(glGetUniformLocation(screenShaderProgram, "screen"), 0);
		glBindVertexArray(vaowo);
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	glfwDestroyWindow(window);
	glfwTerminate();
 }
