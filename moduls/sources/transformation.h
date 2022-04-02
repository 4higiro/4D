// Protection from repeat including
#pragma once

// Including
#include <glad/glad.h>
#include <glm/glm.hpp>

// Load Identity Matrix
void loadIdentity(GLuint program_ID);

// View from (45, 45, 45)* angle
void standartPerspectTransform(GLuint ID);

// Camera class
class camera
{
private:
	glm::vec3 position;
	glm::vec3 target;
	glm::vec3 direction;
	glm::vec3 right;
	glm::vec3 up;
	glm::mat4 view;

	float speed;
public:
	camera();
	void setPos(float x, float y, float z);
	void setTarget(float x, float y, float z);
	void setUp(float x, float y, float z);
	void setSpeed(float speed);
	void dynamic(GLFWwindow* window);
	void update(GLuint ID);
};