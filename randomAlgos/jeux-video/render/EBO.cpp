class EBO
{
private:
public:
    GLuint ID;

    // Incdices are expected to be GLuints
    EBO(GLuint *indices)
    {
        // My dumbass forgot that 4.6 is DSA :P, so gotta remove most of the
        // binds and what not
        // Also btw we dont rlly need any other
        // functions so eh
        glCreateBuffers(1, &ID);
        glNamedBufferData(ID, sizeof(indices), indices, GL_STATIC_DRAW);
    }

    void LinkEBOtoVAO(GLuint& vaoToLink, GLuint& eboToLink)
    {
        glVertexArrayElementBuffer(vaoToLink, eboToLink);
    }

    // my dumbass was about to write a function to
    // get the EBO ID
};
