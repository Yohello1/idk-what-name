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
    void enableArrayIndex(GLuint& vaoIn, GLuint index)
    {
        glEnableVertexArrayAttrib(vaoIn, index);
        glVertexArrayAttribBinding(vaoIn, 0, 0);
    }

    // The offset is the number of floats to make it there
    // NOT THE ACTUAL SIZE
    // As in the offset is multiplied by the size of the a GLfloat
    // So like, offset*sizeof(GLfloat);
    void setArrayFormat(GLuint& vaoIn, GLuint index, GLuint size, GLuint offset)
    {
        glVertexArrayAttribFormat(vaoIn, index, size, GL_FLOAT, GL_FALSE, offset*(sizeof(GLfloat)));
    }

    // You have to compute the stride urself here btw, cause ion have the sanity
    void linkVBO(GLuint& vaoIn, GLuint index, GLuint& vboIn, GLuint offset, GLsizei stride)
    {
        glVertexArrayVertexBuffer(vaoIn, index, vboIn, offset, stride);
    }

    void linkEBO(GLuint& vaoIn, GLuint& eboIn)
    {
        glVertexArrayElementBuffer(vaoIn, eboIn);
    }
};
