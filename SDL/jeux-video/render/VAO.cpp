#include"VAO.h"

VAO::VAO()
{
	glGenVertexArrays(1, &ID);
}

void VAO::LinkVBO(VBO& VBO, GLuint layout)
{
	std::cout << "LAYOUT OF VAO LINKING: " << layout << '\n';
	VBO.Bind();
	glVertexAttribPointer(layout, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
	glEnableVertexAttribArray(layout);
	VBO.Unbind();
}

void VAO::Bind()
{
	glBindVertexArray(ID);
}

void VAO::Unbind()
{
	glBindVertexArray(0);
}

void VAO::Delete()
{
	glDeleteVertexArrays(1, &ID);
}