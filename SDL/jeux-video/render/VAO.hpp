#ifndef VAO_H_
#define VAO_H_

class VAO
{
  private:
  public:

  // The ID of the object
  GLuint ID;

  // Constructor
  VAO();

  void enableArrayIndex(GLuint* vaoIn, GLuint index);
  void setArrayFormat(GLuint* vaoIn, GLuint size, GLuint offset);
};


#endif // VAO_H_
