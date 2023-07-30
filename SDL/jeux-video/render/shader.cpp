// blah blah blah
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

        glDeleteShader(vertShader);
        glDeleteShader(fragShader);
    }

private:

};

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
        GLuint loc = glGetUniformLocation(ID, name);
        if (loc >= 0)
            return loc;
        else
            throw("invalid uniform name");
        return loc;
    }
}
