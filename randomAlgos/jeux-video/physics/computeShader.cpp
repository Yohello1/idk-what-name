class computeShader
{
private:
    void checkCompileErrors()
    {
        int success;
        glGetShaderiv(IDin, 512, NULL, &success);
        if(!success)
        {
            glGetShaderInfoLog(IDin, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        };
    }

public:
    unsigned int ID;

    ComputeShader(const char* computePath)
    {
        // Literally make the shader
        /**
          How do i fold comments?
          ok so I usw vimish to fold
          and unfold comments
          nice

          ok back to the topic at hand
          Im gonna ask abt
          her experiences in da place
          cause I wnana know more abt
          here, like i wanna know
          more abt her story if you
          get what I mean

          Also ion think me and shreya
          are good friends anymore
          we're slowly turning into
          strangers who recoginise
          eachother's voices
        */

        unsigned int compute;

        std::string computeShaderCodeS = getFileContents("physics/computeShader.glsl");
        const char* computeShaderCode = computeShaderCodeS.c_str();

        compute = glShaderSource(compute, 1, &computeShaderCode, NULL);
        glCompileShader(compute);

    }
};
