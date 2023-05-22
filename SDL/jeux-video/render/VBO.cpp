class VBO
{
    private:
    public:

    VBO(GLfloat *vertices, GLsizeiptr size)
    {
        glGenBuffers(1, &ID);
        glBindBuffer(GL_ARRAY_BUFFER, ID);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    }

    void Bind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, ID);
    }

    void UnBind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void Delete() //there should be a `~` here :sob:
    {
        glDeleteBuffers(1, &ID);
    }

};