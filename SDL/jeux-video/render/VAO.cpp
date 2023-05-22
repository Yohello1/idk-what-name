class VAO
{
private:
public:
    // The ID of the object
    GLuint ID;

    // Constructor
    VAO()
    {
        glGenVertexArrays(1, &ID);
    }

    // TODO: automate the below process, cause my dumbass is gonna be lazy af

    // This is self explainatory
    // This is a *special* tool which will help us later ;)
    // BTW you have to get the layout and send it in
    // Hopefully future you set this up by now
    void LinkVBO(VBO &VBO, shader_program program *)
    {
        std::cout << "LAYOUT OF VAO IS BEING LINKED: " << layout << '\n';
        VBO.Bind();

        // Connect the position to the vert attrib
        glEnableVertexAttribArray(program->getUniformID("vertCoords"));
        glVertexAttribPointer(program->getUniformID("vertCoords"), 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), NULL);

        glEnableVertexAttribArray(program->getUniformID("uvCoords"));
        glVertexAttribPointer(program->getUniformID("uvCoords"), 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 3 * sizeof(GLfloat));
    }

    // It tells the gpu that this VAO is in use
    void Bind()
    {
        glBindVertexArray(ID);
    }

    // It tells the gpu that this one is not being used
    void UnBind();
    {
        glBindVertexArray(ID);
    }

    // if you need me to explain this, please off urself
    void Delete(); // I feel like there should be a ~ here
    {
        glDeleteVertexArrays(1, &ID);
    }
};