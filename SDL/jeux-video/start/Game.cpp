// there is no good or evil in the end

class game
{
    public:
        // Tbh most of these dont need to be public but like
        // ig for debugging purposes
        unsigned int ID;
        unsigned int logical_x, logical_y;
        unsigned int window_x, window_y;
        GLuint VAO, VBO, EBO;
        // this does not need to be set in a weird way
        // Verts might, but tbh ion think so, future problem tho
        GLuint indices[6] = {0, 2, 1, 0, 3, 2};
        GLfloat vertices[20] = { -64.0f, -64.0f , 0.0f, 0.0f, 0.0f, -64.0f,  64.0f , 0.0f, 0.0f, 1.0f, 64.0f,  64.0f , 0.0f, 1.0f, 1.0f, 64.0f, -64.0f , 0.0f, 1.0f, 0.0f,}; // omg this line is so long - that's what she said
        Shader inMyMind;


        // x & y are the logical widths
        game(unsigned int id, int x, int y, int scaling, const char* winName)
        {
            ID = id;

            logical_x = x;
            logical_y = y;
            window_x = x*scaling;
            window_x = y*scaling;

            glCreateVertexArrays(1, &VAO);
            glCreateBuffers(1, &VBO);
            glCreateBuffers(1, &EBO);
            glNamedBufferData(VBO, sizeof(vertices), vertices, GL_STATIC_DRAW);
            glNamedBufferData(EBO, sizeof(indices), indices, GL_STATIC_DRAW);
            glEnableVertexArrayAttrib(VAO, 0);
            glVertexArrayAttribBinding(VAO, 0, 0);
            glVertexArrayAttribFormat(VAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
            glEnableVertexArrayAttrib(VAO, 1);
            glVertexArrayAttribBinding(VAO, 1, 0);
            glVertexArrayAttribFormat(VAO, 1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat));
            glVertexArrayVertexBuffer(VAO, 0, VBO, 0, 5 * sizeof(GLfloat));
            glVertexArrayElementBuffer(VAO, EBO);

            inMyMind = new Shader("render/shaders/vert.glsl", "render/shaders/frag.glsl");
        }
    private:

};
