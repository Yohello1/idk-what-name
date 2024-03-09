
namespace Shaders
{
    class computeShader
    {
    public:
      GLuint _ID;

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

        _ID = glCreateProgram();
        glAttachShader(_ID, computeTest);
        glLinkProgram(_ID);
        Shaders::compileErrors(_ID, "PROGRAM");
    }

    /**
      Need I explain what this does?
     (glUseProgram, basically) */
    void useProgram()
    {
        glUseProgram(_ID);
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

    class computeImageOut
    {
    public:
        GLuint _ID;
        uint16_t _width, _height;
        uint8_t _unit; // unit refering to which of the 32 texture slots it occupies
                      // Like 0: in slot 1/32
                      // 1: 2/32
                      // etc etc
      /**
          btw, 2d texture,
          min & mag filters: gl_nearest
          wrap s & t: clamp_to_edge
          rgba32F
          not normalised
          write only */
        computeImageOut(uint16_t x, uint16_t y, uint8_t unit)
        {
            _unit = unit;
            _width = x;
            _height = y;
            glCreateTextures(GL_TEXTURE_2D, 1, &_ID);
            glTextureParameteri(_ID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTextureParameteri(_ID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTextureParameteri(_ID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTextureParameteri(_ID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTextureStorage2D(_ID, 1, GL_RGBA32F, x, y);
            glBindImageTexture(_unit, _ID, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

        }

        GLuint getID()
        {
            return _ID;
        }
    private:
    };


    // compute shader input
    class computeImageIn
    {
    public:
        GLuint _ID;
        uint16_t _width, _height;
        GLuint _unit; // unit refering to which of the 32 texture slots it occupies
                      // Like 0: in slot 1/32
                      // 1: 2/32
                      // etc etc
      /**
          btw, 2d texture,
          min & mag filters: gl_nearest
          wrap s & t: clamp_to_edge
          rgba32F
          not normalised
          write only */
        computeImageIn(uint16_t x, uint16_t y, GLuint unit)
        {
            _width = x;
            _height = y;
            _unit = unit;
            glCreateTextures(GL_TEXTURE_2D, 1, &_ID);
            glTextureParameteri(_ID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTextureParameteri(_ID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTextureParameteri(_ID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTextureParameteri(_ID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTextureStorage2D(_ID, 1, GL_RGBA32F, x, y);
            glBindImageTexture(_unit , _ID, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);

        }

        GLuint getID()
        {
            return _ID;
        }

        /**
          Float data */
        void copyDataFloat(float* rawData)
        {
            glTextureSubImage2D(_ID, 0, 0, 0, _width, _height, GL_RGBA, GL_FLOAT, rawData);
        }

        // ok now lets do the
        // thinking
        // To render we have to copy the data
        // from several 'textures'/maps to the gpu
        // and then it just does the math required
        // to turn that into the final product
        // So what I need is a function which
        // just fetches the data from the cell data
        // type, and from the rgb data type
        // Also needs to just edit a specific rang ofc
        //
        // We'll also need to leave the alpha channel open
        // to being changed, cause like compositing sanity

        // wait this should all be in compute shaders LMAO

    private:
    };
}
