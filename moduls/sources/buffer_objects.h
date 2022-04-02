// Protection from repeat including
#pragma once

// Vertex Buffer Object
class vbo
{
private:
	GLuint ID;
	unsigned int step;
	unsigned int amount;
public:
	vbo(GLfloat* buffer, unsigned int size, unsigned int step);
	void bind();
	GLuint getID();
	int getStep();
	int account();
	~vbo();
};

// Vertex Array Object
class vao
{
private:
	GLuint ID;
	unsigned int countVBO;
	unsigned int amount;
public:
	vao();
	void bind();
	void addVBO(vbo& buffer);
	void draw(GLenum mode);
	GLuint getID();
	~vao();
};

// Element Buffer Object
class ebo
{
private:
	GLuint ID;
	vao arr;
	unsigned int size;
public:
	ebo(vao& arr, GLuint* indexes, unsigned int size);
	void bind();
	void draw(GLenum mode);
	GLuint getID();
	~ebo();
};
