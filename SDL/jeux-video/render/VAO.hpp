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

  // This is self explainatory
  // This is a *special* tool which will help us later ;)
  // void LinkVBO(VBO& VBO, GLuint layout);

  // It tells the gpu that this VAO is in use
  void Bind();

  // It tells the gpu that this one is not being used
  void UnBind();

  // if you need me to explain this, please off urself
  void Delete(); // I feel like there should be a ~ here
};


#endif // VAO_H_
