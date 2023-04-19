#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

std::string get_file_contents(const char *filename);

class Shader
{
public:
	// Reference ID of the Shader Program
	GLuint ID;
	// Constructor that build the Shader Program from 2 different shaders
	Shader(const char *vertexFile, const char *fragmentFile);

	// Activates the Shader Program
	void Activate();
	// Deletes the Shader Program
	void Delete();

	// Time to use ur brain and guess
	void setInt(const char* name, const GLint& value);
	void setUint(const char* name, const GLuint& value);
	void setFloat(const char* name, const GLfloat& value);
	void setVec2(const char* name, const glm::vec2& value);
	void setVec3(const char* name, const glm::vec3& value);
	void setVec4(const char* name, const glm::vec4& value);
	void setMat3(const char* name, const glm::mat3& value);
	void setMat4(const char* name, const glm::mat4& value);

private:
	// Checks if the different Shaders have compiled properly
	void compileErrors(unsigned int shader, const char *type);
	GLint getUniformID(const char* name);
	GLint loc;
};

#endif