#ifndef EBO_H_
#define EBO_H_

class EBO
{
    public:
    GLuint ID;
      EBO(GLuint* indices, GLsizeiprt size);

      void LinkEBOtoVAO(GLuint* vaoToLink, GLuint* eboToLink);
};


#endif // EBO_H_
