// Including
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>

#include "shaders.h"
#include "buffer_objects.h"
#include "transformation.h"
#include "tesseract.h"

// Size of window
#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600

// Callback function GLFW
void sizeCallback(GLFWwindow* window, int width, int height);

// General keyboard event proccesor
void processKeyboardEvents(GLFWwindow* window);

// Namespaces
using namespace std;

// Input in program
int main()
{
	// Add russion language
	setlocale(LC_ALL, "Ru");

	// GLFW init
	if (!glfwInit())
		return -1;

	// Set version OpenGL 3.3 Core Profile
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create graphic window
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "GRAPHICS WINDOW", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	// Make OpenGL context for window
	glfwMakeContextCurrent(window);

	// GLAD init
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		glfwTerminate();
		return -1;
	}

	// Limitation viewport
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glfwSetFramebufferSizeCallback(window, sizeCallback);

	// Load shaders for color rendering
	vertex_shader color_vertex_shader;
	string path = COLOR_SHADER_PATH + (string)"vertex_shader.glsl";
	loadShader(path.c_str(), &color_vertex_shader);
	fragment_shader color_fragment_shader;
	path = COLOR_SHADER_PATH + (string)"fragment_shader.glsl";
	loadShader(path.c_str(), &color_fragment_shader);

	// Create shader program for color rendering
	shader_program color_shader_program(color_vertex_shader, color_fragment_shader);

	// Using shader program
	color_shader_program.use();

	// Start transformation
	loadIdentity(color_shader_program.getID());
	standartPerspectTransform(color_shader_program.getID());

	/////////////////////////////////////////////////////////////////////////////////////////////////

	GLfloat values[12] = {
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};

	GLuint indexes[6] = {
		0, 1, 0, 2, 0, 3
	};

	vbo lines_coords(values, 12, 3);
	vbo lines_colors(values, 12, 3);
	vao lines_data;
	lines_data.addVBO(lines_coords);
	lines_data.addVBO(lines_colors);
	ebo lines(lines_data, indexes, 6);
	
	tesseract t;
	t.print4D();
	t.sampling(1);
	cout << endl << endl;
	t.print3D();

	camera c;
	c.setPos(1, 1, 1);

	/////////////////////////////////////////////////////////////////////////////////////////////////

	// Rendering
	float thetta = 0;

	while (!glfwWindowShouldClose(window))
	{
		processKeyboardEvents(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//float x = sin(thetta / 100) * 3;
		//float y = cos(thetta / 100) * 3;
		//c.setPos(cumX, cumY, 0.0f);
		//c.setTarget(0.0f, 0.0f, 0.0f);
		//c.setUp(0.0f, 0.0f, 1.0f);
		c.dynamic(window);
		c.update(color_shader_program.getID());

		lines.draw(GL_LINES);

		glfwSwapBuffers(window);
		glfwPollEvents();

		thetta += 0.01;
	}

	// Resources clear
	glfwTerminate();
	return 0;
}

// General keyboard event proccesor
void processKeyboardEvents(GLFWwindow* window)
{
	// Cloose window (ESCAPE)
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// Callback function GLFW
void sizeCallback(GLFWwindow* window, int width, int height)
{
	// Change window size
	glViewport(0, 0, width, height);
}