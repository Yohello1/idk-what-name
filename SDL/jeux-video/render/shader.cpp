// blah blah blah
class Shader
{
    public:
    GLuint shaderProgram;
    // btw it is expecting the file paths for the glsl shaders
    Shader(const char *vertFile, const char* fragFile)
    {
        std::cout << "hello weridos\n";
        const char* vertCode = getFileContents(vertFile);
        const char* fragCode = getFileContents(fragFile);

        std::cout << "i got the files :3\n";

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
