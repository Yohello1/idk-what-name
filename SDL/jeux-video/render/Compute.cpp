
namespace Shaders
{
    class computeShader
    {
    public:
      GLuint ID;

    computeShader(const char* computeShaderSource)
    {
        // get file contents & convert
        std::string computeShaderCodeS = getFileContents(computeShaderSource);
        const char* computeShaderCode = computeShaderCodeS.c_str();

        // compile the shader
        GLuint computeTest = glCreateShader(GL_COMPUTE_SHADER);
        glShaderSource (computeTest, 1, &computeShaderCode, NULL);
        glCompileShader(computeTest);
        Shaders::compileErrors(computeTest, "COMPUTE");

        ID = glCreateProgram();
        glAttachShader(ID, computeTest);
        glLinkProgram(ID);
        Shaders::compileErrors(ID, "PROGRAM");
    }

    /**
      Need I explain what this does?
     (glUseProgram, basically) */
    void useProgram()
    {
        glUseProgram(ID);
    }
    private:
    };

    class computeImage
    {

    };
}
