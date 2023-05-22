#ifndef VAO_H_
#define VAO_H_

class VBO
{
    private:
    public:
    GLuint ID;

    VBO(GLfloat *vertices, GLsizeiptr size);

    void Bind();
    void Unbind();
    void Delete();
    
};

#endif // VAO_H_
