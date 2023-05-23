#ifndef EBO_H_
#define EBO_H_

class EBO
{
    public:
    GLuint ID;
    EBO(GLuint* indices, GLsizeiprt size);

    void Bind();
    void Unbind();
    void Delete(); // once again i think there should be a `~` here


    private:
};


#endif // EBO_H_
