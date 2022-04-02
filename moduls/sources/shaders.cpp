// Including
#include <glad/glad.h>
#include <iostream>
#include <fstream>
#include <string>

#include "shaders.h"

// Namescpaces
using namespace std;

// Load Shader
void loadShader(const char* path, shader* sh)
{
	ifstream file;
	string current;

	const char* source;

	try
	{
		file.open(path);
		if (!file.is_open())
			throw - 1;

		current = "";
		while (!file.eof())
		{
			current += file.get();
		}
		source = current.c_str();

		sh->changeShader(source);
	}
	catch (int e)
	{
		if (e == -1)
		{
			cout << "ERROR. FILE NOT OPEN" << endl;
		}
	}
}

// Construct shader (defoult)
shader::shader()
{
	ID = 0;
	compileState = false;
	source = "";
}

// Construct shader
shader::shader(const char* source)
{
	ID = 0;
	compileState = false;
	this->source = source;
}

// Compile status getter
bool shader::checkCompile()
{
	return compileState;
}

// ID getter
GLuint shader::getID()
{
	return ID;
}

// New source for shader
void shader::changeShader(const char* source)
{
	this->source = source;
	compile();
}

// Vertex shader compile
void vertex_shader::compile()
{
	GLint succes;
	char infoLog[512];

	ID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(ID, 1, &source, NULL);
	glCompileShader(ID);

	glGetShaderiv(ID, GL_COMPILE_STATUS, &succes);
	if (!succes)
	{
		glGetShaderInfoLog(ID, 512, NULL, infoLog);
		cout << "ERROR VERTEX SHADER COMPILE\n" << infoLog << endl;
		compileState = false;
	}
	else 
	{
		compileState = true;
	}
}

// Destruct vertex shader
vertex_shader::~vertex_shader()
{
	glDeleteShader(ID);
}

// fragment shader compile
void fragment_shader::compile()
{
	GLint succes;
	char infoLog[512];

	ID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(ID, 1, &source, NULL);
	glCompileShader(ID);

	glGetShaderiv(ID, GL_COMPILE_STATUS, &succes);
	if (!succes)
	{
		glGetShaderInfoLog(ID, 512, NULL, infoLog);
		cout << "ERROR FRAGMENT SHADER COMPILE\n" << infoLog << endl;
		compileState = false;
	}
	else
	{
		compileState = true;
	}
}

// Destruct fragment shader
fragment_shader::~fragment_shader()
{
	glDeleteShader(ID);
}

// Construct shader program
shader_program::shader_program(vertex_shader& vs, fragment_shader& fs)
{
	GLuint vsID = vs.getID();
	GLuint fsID = fs.getID();

	ID = glCreateProgram();

	glAttachShader(ID, vsID);
	glAttachShader(ID, fsID);
	glLinkProgram(ID);

	GLint succes;
	char infoLog[512];

	glGetProgramiv(ID, GL_LINK_STATUS, &succes);
	if (!succes) {
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		cout << "ERROR SHADER PROGRAM LINK\n" << infoLog << endl;
		compileState = false;
	}
	else
	{
		compileState = true;
	}
}

// Compile status getter
bool shader_program::checkCompile()
{
	return compileState;
}

// Using shader program
void shader_program::use()
{
	glUseProgram(ID);
}

// ID getter
GLuint shader_program::getID()
{
	return ID;
}

// Destruct shader
shader_program::~shader_program()
{
	glDeleteProgram(ID);
}
