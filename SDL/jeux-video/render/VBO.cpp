class VBO
{
    private:
    public:
    GLuint ID;

    VBO(GLfloat *vertices)
    {
        glCreateBuffers(1, &ID);
        glNamedBufferData(ID, sizeof(vertices), vertices, GL_STATIC_DRAW);

    }


};
