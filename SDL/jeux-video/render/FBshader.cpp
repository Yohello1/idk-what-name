class FBShader {
public:
    // FB stands for frame buffer
    GLuint FBShaderProgram;
    FBShader(const char* FBvertFile, const char* FBfragFile)
    {
        GLuint FBvertShader;
        GLuint FBfragShader;

        const char* FBvertCode = getFileContents(FBvertFile);
        const char* FBfragCode = getFileContents(FBfragFile);

        FBvertShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(FBvertShader, 1, &FBvertCode, NULL);
        glCompileShader(FBvertShader);

        FBfragShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(FBfragShader, 1, &FBfragCode, NULL);
        glCompileShader(FBfragShader);

        FBShaderProgram = glCreateProgram();
        glAttachShader(FBShaderProgram, FBvertShader);
        glAttachShader(FBShaderProgram, FBfragShader);

        glDeleteShader(FBvertShader);
        glDeleteShader(FBfragShader);
    }
};
