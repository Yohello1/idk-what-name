class VAO
{
private:
public:
    // The ID of the object
    GLuint ID;

    // Constructor
    VAO()
    {
        glCreateVertexArrays(1, &ID);
    }

    // Btw it's assumed that the index-buffer-binding
    // which is associated to the generic
    // vertex attribute
    // is 0, cause ion have the sanity
    void enableArrayIndex(VAO& vaoIn, GLuint index)
    {
        glEnableVertexArrayAttrib(vaoIn.ID, index);
        glVertexArrayAttribBinding(vaoIn.ID, index, 0);
    }

    // The offset is the number of floats to make it there
    // NOT THE ACTUAL SIZE
    // Like offset*sizeof(GLfloat)
    void setArrayFormat(VAO& vaoIn, GLuint index, GLuint size, GLuint offset)
    {
        glVertexArrayAttribFormat(vaoIn.ID, index, size, GL_FLOAT, GL_FALSE, offset*(sizeof(GLfloat)));
    }

    // You have to compute the stride urself here btw, cause ion have the sanity
    void linkVBO(VAO& vaoIn, GLuint index, VBO& vboIn, GLuint offset, GLsizei stride)
    {
        glVertexArrayVertexBuffer(vaoIn.ID, index, vboIn.ID, offset, stride);
    }

    void linkEBO(VAO& vaoIn, EBO& eboIn)
    {
        glVertexArrayElementBuffer(vaoIn.ID, eboIn.ID);
    }
};
