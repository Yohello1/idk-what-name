
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

    void printMaxComputeSize()
    {
        int work_grp_cnt[3];
        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &work_grp_cnt[0]);
        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &work_grp_cnt[1]);
        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &work_grp_cnt[2]);
        std::cout << "Max work groups per compute shader" <<
                     " x:"                                << work_grp_cnt[0] <<
                     " y:"                                << work_grp_cnt[1] <<
                     " z:"                                << work_grp_cnt[2] << "\n";

        int work_grp_size[3];
        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &work_grp_size[0]);
        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &work_grp_size[1]);
        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &work_grp_size[2]);
	    std::cout << "Max work group sizes" <<
		             " x:"                  << work_grp_size[0] <<
		             " y:"                  << work_grp_size[1] <<
		             " z:"                  << work_grp_size[2] << "\n";
    }
    private:
    };

    class computeImage
    {
    public:
        GLuint ID;
        /**
          btw, 2d texture,
          min & mag filters: gl_nearest
          wrap s & t: clamp_to_edge
          rgba32F
          not normalised
          write only */
        computeImage(uint16_t x, uint16_t y)
        {
            glCreateTextures(GL_TEXTURE_2D, 1, &ID);
            glTextureParameteri(ID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTextureParameteri(ID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTextureParameteri(ID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTextureParameteri(ID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTextureStorage2D(ID, 1, GL_RGBA32F, x, y);
            glBindImageTexture(0, ID, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

        }

        GLuint getID()
        {
            return ID;
        }
    private:
    };
}
