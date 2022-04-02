// Including
#include <glad/glad.h>
#include <iostream>

#include "buffer_objects.h"

// Construct VBO
vbo::vbo(GLfloat* buffer, unsigned int size, unsigned int step)
{
	this->step = step;
	this->amount = size / step;

	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), buffer, GL_STATIC_DRAW);
}

// Bind VBO
void vbo::bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

// ID getter
GLuint vbo::getID()
{
	return ID;
}

// Step getter
int vbo::getStep()
{
	return step;
}

// Size getter
int vbo::account()
{
	return amount;
}

// Destruct VBO
vbo::~vbo()
{
	glDeleteBuffers(1, &ID);
}

// Construct VAO
vao::vao()
{
	countVBO = 0;
	amount = 0;
	glGenVertexArrays(1, &ID);
}

// Bind VAO
void vao::bind()
{
	glBindVertexArray(ID);
}

// Add VBO
void vao::addVBO(vbo& buffer)
{
	if (amount == buffer.account() || amount == 0)
	{
		unsigned int step = buffer.getStep();
		amount = buffer.account();

		glBindVertexArray(ID);
		glEnableVertexAttribArray(countVBO);
		buffer.bind();
		glVertexAttribPointer(countVBO, amount, GL_FLOAT, GL_FALSE, step * sizeof(float), (void*)0);

		countVBO++;
	}
}

// VAO rendering
void vao::draw(GLenum mode)
{
	glBindVertexArray(ID);
	glDrawArrays(mode, 0, amount);
}

// ID getter
GLuint vao::getID()
{
	return ID;
}

// Destruct VAO
vao::~vao()
{
	glDeleteVertexArrays(1, &ID);
}

// Construct EBO
ebo::ebo(vao& arr, GLuint* indexes, unsigned int size)
{
	this->arr = arr;
	this->size = size;

	arr.bind();
	
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(int), indexes, GL_STATIC_DRAW);
}

// Bind EBO
void ebo::bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

// EBO rendering
void ebo::draw(GLenum mode)
{
	arr.bind();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
	glDrawElements(mode, size, GL_UNSIGNED_INT, 0);
}

// ID getter
GLuint ebo::getID()
{
	return ID;
}

// Destruct EBO
ebo::~ebo()
{
	glDeleteBuffers(1, &ID);
}
