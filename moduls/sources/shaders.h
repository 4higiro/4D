// Protection from repeat including
#pragma once

// Shaders path
#define COLOR_SHADER_PATH "shaders/color_shaders/"
#define TEXTURE_SHADER_PATH "shaders/texture_shaders/"

// Abstract shader
class shader
{
protected:
	const char* source;
	GLuint ID;
	bool compileState;
public:
	shader();
	shader(const char* source);
	virtual void compile() = 0;
	bool checkCompile();
	GLuint getID();
	void changeShader(const char* source);
};

// Load Shader
void loadShader(const char* path, shader* shader);

// Vertex shader
class vertex_shader : public shader
{
public:
	void compile();
	~vertex_shader();
};

// Fragment shader
class fragment_shader : public shader
{
public:
	void compile();
	~fragment_shader();
};

// Shader program
class shader_program
{
private:
	GLuint ID;
	bool compileState;
public:
	shader_program(vertex_shader& vs, fragment_shader& fs);
	bool checkCompile();
	void use();
	GLuint getID();
	~shader_program();
};