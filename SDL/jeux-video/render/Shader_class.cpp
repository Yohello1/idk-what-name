#include "shader_class.h"

std::string get_file_contents(const char *filename)
{
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return (contents);
	}
	throw(errno);
}

Shader::Shader(const char *vertexFile, const char *FragFile)
{
	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragCode = get_file_contents(FragFile);

	const char *vertexSource = vertexCode.c_str();
	const char *fragSource = fragCode.c_str();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	compileErrors(vertexShader, "VERTEX");

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragSource, NULL);
	glCompileShader(fragmentShader);
	compileErrors(fragmentShader, "FRAGMENT");

	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::Activate()
{
	glUseProgram(ID);
}

void Shader::Delete()
{
	glDeleteProgram(ID);
}

void Shader::setInt(const char *name, const GLint &value)
{

	loc = getUniformID(name);
	glUniform1iv(loc, 1, &value);
}
void Shader::setUint(const char *name, const GLuint &value)
{
	loc = getUniformID(name);
	glUniform1uiv(loc, 1, &value);
}
void Shader::setFloat(const char *name, const GLfloat &value)
{
	loc = getUniformID(name);
	glUniform1fv(loc, 1, &value);
}
void Shader::setVec2(const char *name, const glm::vec2 &value)
{
	loc = getUniformID(name);
	glUniform2fv(loc, 1, &value[0]);
}
void Shader::setVec3(const char *name, const glm::vec3 &value)
{
	loc = getUniformID(name);
	glUniform3fv(loc, 1, &value[0]);
}
void Shader::setVec4(const char *name, const glm::vec4 &value)
{
	loc = getUniformID(name);
	glUniform4fv(loc, 1, &value[0]);
}
void Shader::setMat3(const char *name, const glm::mat3 &value)
{
	loc = getUniformID(name);
	glUniformMatrix3fv(loc, 1, GL_FALSE, &value[0][0]);
}
void Shader::setMat4(const char *name, const glm::mat4 &value)
{
	loc = getUniformID(name);
	glUniformMatrix4fv(loc, 1, GL_FALSE, &value[0][0]);
}

void Shader::compileErrors(unsigned int shader, const char *type)
{
	// Stores status of compilation
	GLint hasCompiled;
	// Character array to store error message in
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_COMPILATION_ERROR for:" << type << "\n"
					  << infoLog << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_LINKING_ERROR for:" << type << "\n"
					  << infoLog << std::endl;
		}
	}

	/* pfffffft who needs to check for errors
		Like who needs fake friends?
		It hurts to cut them off, but it needs to be done
		Lest they hurt you more
		*/
}

GLint Shader::getUniformID(const char *name)
{
	loc = glGetUniformLocation(ID, name);
	if (loc >= 0)
	{
		return loc;
	}
	else
	{
		std::cout << "location too small " << '\n';
		throw("invalid uniform name");
	}
	return loc;
}
