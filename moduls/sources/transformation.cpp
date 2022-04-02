// Including
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "transformation.h"

// Using names
using namespace glm;

// Load identity matrix
void loadIdentity(GLuint program_ID)
{
	mat4 m = mat4(1.0f);

	glUniformMatrix4fv(glGetUniformLocation(program_ID, "model"), 1, GL_FALSE, value_ptr(m));
}

// View from (45, 45, 45)* angle
void standartPerspectTransform(GLuint ID)
{
	mat4 m = mat4(1.0f);
	m = rotate(m, radians(285.0f), vec3(1.0f, 0.0f, 0.0f));
	m = rotate(m, radians(225.0f), vec3(0.0f, 0.0f, 1.0f));

	glUniformMatrix4fv(glGetUniformLocation(ID, "model"), 1, GL_FALSE, value_ptr(m));
}

// Construct camera
camera::camera()
{
	position = vec3(0.0f, 0.0f, 2.0f);
	target = vec3(0.0f, 0.0f, 0.0f);
	direction = normalize(position - target);
	
	vec3 current = vec3(0.0f, 1.0f, 0.0f);
	right = normalize(cross(current, direction));
	up = cross(direction, right);

	view = lookAt(position, target, up);

	speed = 0.0005f;
}

// Position setter
void camera::setPos(float x, float y, float z)
{
	position = vec3(x, y, z);
	view = lookAt(position, target, up);
}

// Target setter
void camera::setTarget(float x, float y, float z)
{
	target = vec3(x, y, z);
	view = lookAt(position, target, up);
}

// Setter of up-vector
void camera::setUp(float x, float y, float z)
{
	up = vec3(x, y, z);
	view = lookAt(position, target, up);
}

// Speed of moving camera setter
void camera::setSpeed(float speed)
{
	this->speed = speed;
}

// Move camera
void camera::dynamic(GLFWwindow* window)
{
	vec3 front = vec3(1.0f, 0.0f, 0.0f);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		position += speed * front;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		position -= speed * front;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		position -= normalize(cross(front, up)) * speed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		position += normalize(cross(front, up)) * speed;

	view = lookAt(position, target, up);
}

// Initialisation LookAt matrix of camera
void camera::update(GLuint ID)
{
	mat4 model = mat4(1.0f);
	glUniformMatrix4fv(glGetUniformLocation(ID, "model"), 1, GL_FALSE, value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(ID, "view"), 1, GL_FALSE, value_ptr(view));
	mat4 projection = perspective(radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
	glUniformMatrix4fv(glGetUniformLocation(ID, "projection"), 1, GL_FALSE, value_ptr(projection));
}
