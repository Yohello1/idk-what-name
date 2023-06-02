class FBShader {
public:
    // FB stands for frame buffer
    GLuint ID;
    FBShader(const char* FBvertFile, const char* FBfragFile)
    {
        GLuint FBvertShader;
        GLuint FBfragShader;

        std::string FBvertCodeS = getFileContents(FBvertFile);
        std::string FBfragCodeS = getFileContents(FBfragFile);

        const char* FBvertCode = FBvertCodeS.c_str();
        const char* FBfragCode = FBfragCodeS.c_str();


        FBvertShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(FBvertShader, 1, &FBvertCode, NULL);
        glCompileShader(FBvertShader);

        FBfragShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(FBfragShader, 1, &FBfragCode, NULL);
        glCompileShader(FBfragShader);

        ID = glCreateProgram();
        glAttachShader(ID, FBvertShader);
        glAttachShader(ID, FBfragShader);

        glDeleteShader(FBvertShader);
        glDeleteShader(FBfragShader);
    }
};
