#version 330 core
layout (location = 0) in vec3 vertexCoords;
layout (location = 1) in vec3 vertexColors;
out vec3 color;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main() 
{
	color = vertexColors;
	gl_Position = projection * view * model * vec4(vertexCoords, 1.0f);
}