#include "VBO.hpp"

class VBO
{
    private:
    public:

    VBO(GLfloat *vertices, GLsizeiptr size)
    {
        glCreateBuffers(1, &VBO);
        glNamedBufferData(VBO, sizeof(vertices), vertices, GL_STATIC_DRAW);

    }


};
