namespace rendering
{

// Vertex Shader source code
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
//Fragment Shader source code
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
"}\n\0";





    // printShaderInfoLog
    // From OpenGL Shading Language 3rd Edition, p215-216
    // Display (hopefully) useful error messages if shader fails to compile
    void printShaderInfoLog(GLint shader)
    {
        int infoLogLen = 0;
        int charsWritten = 0;
        GLchar *infoLog;

        // Kowalski status report
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen);

        // check for more errors

        if (infoLogLen > 0)
        {
            infoLog = new GLchar[infoLogLen];
            // did it allocate memory
            glGetShaderInfoLog(shader, infoLogLen, &charsWritten, infoLog);
            std::cout << "InfoLog:" << '\n'
                      << infoLog << '\n';
            delete[] infoLog;
        }

        /* pfffffft who needs to check for errors
        Like who needs fake friends?
        It hurts to cut them off, but it needs to be done
        Lest they hurt you more
        */
    }

    // loadFile - loads text file into char* fname
    // allocates memory - so need to delete after use
    // size of file returned in fSize
    bool read_n_compile_shader(const char *filename, GLuint &hdlr, GLenum shaderType)
    {
        std::ifstream is(filename, std::ios::in | std::ios::binary | std::ios::ate);
        if (!is.is_open())
        {
            std::cerr << "Unable to open file " << filename << std::endl;
            return false;
        }
        long size = is.tellg();
        char *buffer = new char[size + 1];
        is.seekg(0, std::ios::beg);
        is.read(buffer, size);
        is.close();
        buffer[size] = 0;

        hdlr = glCreateShader(shaderType);
        glShaderSource(hdlr, 1, (const GLchar **)&buffer, NULL);
        glCompileShader(hdlr);
        std::cerr << "info log for " << filename << std::endl;
        printShaderInfoLog(hdlr);
        delete[] buffer;
        return true;
    }

    void setShaders(GLuint &prog_hdlr, const char *vsfile, const char *fsfile)
    {
        GLuint vert_hdlr, frag_hdlr;
        read_n_compile_shader(vsfile, vert_hdlr, GL_VERTEX_SHADER);
        read_n_compile_shader(fsfile, frag_hdlr, GL_FRAGMENT_SHADER);

        prog_hdlr = glCreateProgram();
        glAttachShader(prog_hdlr, frag_hdlr);
        glAttachShader(prog_hdlr, vert_hdlr);

        glLinkProgram(prog_hdlr);
        std::cerr << "info log for the linked program" << std::endl;
        printShaderInfoLog(prog_hdlr);
    }

}