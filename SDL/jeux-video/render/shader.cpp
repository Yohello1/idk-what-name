namespace Shaders
{

    void compileErrors(unsigned int shader, const char* type)
    {
        // Stores status of compilation
        GLint hasCompiled;
        // Character array to store error message in
        char infoLog[1024];
        // I should really fix this logic sooner or later
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
            if (hasCompiled == GL_FALSE)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "SHADER_COMPILATION_ERROR for:" << type << "\n" << infoLog << std::endl;
            }
        }
        else if (type != "COMPUTE")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
            if (hasCompiled == GL_FALSE)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "SHADER_COMPILATION_ERROR for:" << type << "\n" << infoLog << std::endl;
            }

        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled);
            if (hasCompiled == GL_FALSE)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "SHADER_LINKING_ERROR for:" << type << "\n" << infoLog << std::endl;
            }
        }

	/* pfffffft who needs to check for errors
        Like who needs fake friends?
        It hurts to cut them off, but it needs to be done
        Lest they hurt you more
        */
    }

    // ok to be clear, this has never been tested :P
    GLint getUniformID(const char* name, GLuint ID) {
        GLuint shaderProgram = glGetUniformLocation(ID, name);
        if (shaderProgram >= 0)
            return shaderProgram;
        else
            throw("invalid uniform name");
        return shaderProgram;
    }
}

class Shader
{
    public:
    GLuint shaderProgram;
    // btw it is expecting the file paths for the glsl shaders
    Shader(const char *vertFile, const char* fragFile)
    {
        std::string vertCodeS = getFileContents(vertFile);
        std::string fragCodeS = getFileContents(fragFile);

        const char* vertCode = vertCodeS.c_str();
        const char* fragCode = fragCodeS.c_str();

        GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
        // Why is this a GLint instead of GLuint
        // You cannot have negative sized arrays
        glShaderSource(vertShader, 1, &vertCode, NULL);
        glCompileShader(vertShader);

        GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragShader, 1, &fragCode, NULL);
        glCompileShader(fragShader);

        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertShader);
        glAttachShader(shaderProgram, fragShader);
        glLinkProgram(shaderProgram);

        Shaders::compileErrors(shaderProgram, "PROGRAM");

        glDeleteShader(vertShader);
        glDeleteShader(fragShader);
    }

    // tell me what u hate me about me

    void setInt(const char *name, const GLint &value)
    {

        GLuint loc = getUniformID(name);
        glUniform1iv(loc, 1, &value);
    }
    void setUint(const char *name, const GLuint &value)
    {
        GLuint loc = getUniformID(name);
        glUniform1uiv(loc, 1, &value);
    }
    void setFloat(const char *name, const GLfloat &value)
    {
        GLuint loc = getUniformID(name);
        glUniform1fv(loc, 1, &value);
    }
    void setVec2(const char *name, const glm::vec2 &value)
    {
        GLuint loc = getUniformID(name);
        glUniform2fv(loc, 1, &value[0]);
    }
    void setVec3(const char *name, const glm::vec3 &value)
    {
        GLuint loc = getUniformID(name);
        glUniform3fv(loc, 1, &value[0]);
    }
    void setVec4(const char *name, const glm::vec4 &value)
    {
        GLuint loc = getUniformID(name);
        glUniform4fv(loc, 1, &value[0]);
    }
    void setMat3(const char *name, const glm::mat3 &value)
    {
        GLuint loc = getUniformID(name);
        glUniformMatrix3fv(loc, 1, GL_FALSE, &value[0][0]);
    }
    void setMat4(const char *name, const glm::mat4 &value)
    {
        GLuint loc = getUniformID(name);
        glUniformMatrix4fv(loc, 1, GL_FALSE, &value[0][0]);
    }

    GLint getUniformID(const char *name)
    {
        GLuint loc = glGetUniformLocation(shaderProgram, name);
        if (loc >= 0)
            {
                return loc;
            }
        else
            {
                /*
                  it this is ever written out it's because it's trying to declare a variable which doesnt exist in the shaders
                  Or the variable does exist, and it is unused, thus opengl discards it while the shadre code is compiled
                  so it doesnt exist in the end */
                std::cout << "shaderProgramation too small " << '\n';
                std::cout << loc << '\n';
                throw("invalid uniform name");
            }
    }

private:

};
