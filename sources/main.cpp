#include <SFML/Graphics.hpp>
using namespace sf;

#include <iostream>
using namespace std;

float rad(float angle)
{
	float pi = acos(-1);
	return angle * pi / 180.0f;
}

float* multipleMat3f(float* a, float* b, float*& result)
{
	result = new float[9];
	for (int i = 0; i < 9; i++)
		result[i] = 0.0f;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 3; k++)
				result[i * 3 + j] += a[i * 3 + k] * b[k * 3 + j];
		}
	}
	return result;
}

float* multipleMat4f(float* a, float* b, float*& result)
{
	result = new float[16];
	for (int i = 0; i < 16; i++)
		result[i] = 0.0f;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
				result[i * 4 + j] += a[i * 4 + k] * b[k * 4 + j];
		}
	}
	return result;
}

class vec3
{
public:
	float x, y, z;

	vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
	vec3(float value) : x(value), y(value), z(value) {}

	vec3 operator+(const vec3& other) { return vec3(this->x + other.x, this->y + other.y, this->z + other.z); };
	vec3 operator-(const vec3& other) { return vec3(this->x - other.x, this->y - other.y, this->z - other.z); };
	vec3 operator*(const vec3& other) { return vec3(this->x * other.x, this->y * other.y, this->z * other.z); };
	vec3 operator/(const vec3& other) { return vec3(this->x / other.x, this->y / other.y, this->z / other.z); };
};

class vec4
{
public:
	float x, y, z, w;

	vec4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
	vec4(float value) : x(value), y(value), z(value), w(value) {}

	vec3 xyz = vec3(x, y, z);

	vec4 operator+(const vec4& other) { return vec4(this->x + other.x, this->y + other.y, this->z + other.z, this->w + other.w); };
	vec4 operator-(const vec4& other) { return vec4(this->x - other.x, this->y - other.y, this->z - other.z, this->w - other.w); };
	vec4 operator*(const vec4& other) { return vec4(this->x * other.x, this->y * other.y, this->z * other.z, this->w * other.w); };
	vec4 operator/(const vec4& other) { return vec4(this->x / other.x, this->y / other.y, this->z / other.z, this->w / other.w); };
};

float dot(const vec3& a, const vec3& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

float dot(const vec4& a, const vec4& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

float length(const vec3& v)
{
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

float length(const vec4& v)
{
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
}

#define RELEASE_ENGINE

int main()
{
#ifdef RELEASE_ENGINE
	Vector2f resolution = Vector2f(1280, 720);
	RenderWindow window(VideoMode(resolution.x, resolution.y), "Engine");
	VideoMode vm = VideoMode::getDesktopMode();
	window.setFramerateLimit(60);
	window.setMouseCursorVisible(false);

	RenderTexture texture;
	texture.create(resolution.x, resolution.y);
	Sprite sprite = Sprite(texture.getTexture());

	Shader shader;
	shader.loadFromFile("shaders/fragment_shader.glsl", sf::Shader::Fragment);
	shader.setUniform("resolution", resolution);

	float* matrix = new float[16];
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			matrix[i * 4 + j] = 0.0f;
		matrix[i * 4 + i] = 1.0f;
	}
	Glsl::Mat4 view(matrix);
	shader.setUniform("view", view);

	Glsl::Mat4 transform(matrix);
	shader.setUniform("transform", transform);

	Vector3f position(0.0f, 0.0f, -2.0f);
	shader.setUniform("position", position);

	float w = 0.0f;
	shader.setUniform("w", w);

	int mouse_x = resolution.x / 2;
	int mouse_y = resolution.y / 2;
	Mouse::setPosition(Vector2i(resolution.x / 2, resolution.y / 2), window);

	float angle_4d = 0.0f;

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();

			if (event.type == Event::KeyPressed)
			{
				if (event.key.code == Keyboard::Escape)
					window.close();

				if (event.key.code == Keyboard::LShift)
					window.setMouseCursorVisible(true);

				if (event.key.code == Keyboard::W)
				{
					position.x += matrix[2] / sqrt(matrix[2] * matrix[2] + matrix[10] * matrix[10] + 0.01f) / 10.0f;
					position.z += matrix[10] / sqrt(matrix[2] * matrix[2] + matrix[10] * matrix[10] + 0.01f) / 10.0f;
				}

				if (event.key.code == Keyboard::S)
				{
					position.x -= matrix[2] / sqrt(matrix[2] * matrix[2] + matrix[10] * matrix[10] + 0.01f) / 10.0f;
					position.z -= matrix[10] / sqrt(matrix[2] * matrix[2] + matrix[10] * matrix[10] + 0.01f) / 10.0f;
				}

				if (event.key.code == Keyboard::D)
				{
					position.x += matrix[0] / sqrt(matrix[0] * matrix[0] + matrix[8] * matrix[8] + 0.01f) / 10.0f;
					position.z += matrix[8] / sqrt(matrix[0] * matrix[0] + matrix[8] * matrix[8] + 0.01f) / 10.0f;
				}

				if (event.key.code == Keyboard::A)
				{
					position.x -= matrix[0] / sqrt(matrix[0] * matrix[0] + matrix[8] * matrix[8] + 0.01f) / 10.0f;
					position.z -= matrix[8] / sqrt(matrix[0] * matrix[0] + matrix[8] * matrix[8] + 0.01f) / 10.0f;
				}

				if (event.key.code == Keyboard::Q)
					position.y += 1.0f / 10.0f;
				if (event.key.code == Keyboard::E)
					position.y -= 1.0f / 10.0f;

				if (event.key.code == Keyboard::R)
					w += 0.01f;
				if (event.key.code == Keyboard::F)
					w -= 0.01f;

				if (event.key.code == Keyboard::T)
					angle_4d += 0.01f;
				if (event.key.code == Keyboard::G)
					angle_4d -= 0.01f;

				shader.setUniform("position", position);
				shader.setUniform("w", w);

				float* transform_4d1 = new float[16];
				float* transform_4d2 = new float[16];

				float transform_4d_x[16] = {
					cos(angle_4d), 0.0f, 0.0f, -sin(angle_4d),
					0.0f, 1.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f, 0.0f,
					sin(angle_4d), 0.0f, 0.0f, cos(angle_4d)
				};

				float transform_4d_y[16] = {
					1.0f, 0.0f, 0.0f, 0.0f,
					0.0f, cos(angle_4d), 0.0f, -sin(angle_4d),
					0.0f, 0.0f, 1.0f, 0.0f,
					0.0f, sin(angle_4d), 0.0f, cos(angle_4d)
				};

				float transform_4d_z[16] = {
					1.0f, 0.0f, 0.0f, 0.0f,
					0.0f, 1.0f, 0.0f, 0.0f,
					0.0f, 0.0f, cos(angle_4d), -sin(angle_4d),
					0.0f, 0.0f, sin(angle_4d), cos(angle_4d)
				};

				multipleMat4f(transform_4d_x, transform_4d_y, transform_4d1);
				multipleMat4f(transform_4d_z, transform_4d1, transform_4d2);
				transform = Glsl::Mat4(transform_4d2);
				shader.setUniform("transform", transform);

				delete[] transform_4d1;
				delete[] transform_4d2;
			}

			if (event.type == Event::MouseMoved)
			{
				int mouse_pos[2] = {
					event.mouseMove.x - resolution.x / 2,
					event.mouseMove.y - resolution.y / 2
				};
				mouse_x += mouse_pos[0];
				mouse_y += mouse_pos[1];
				float angle_x = rad(mouse_x / 15);
				float angle_y = rad(mouse_y / 15);
				float rotate_x[16] = {
					cos(-angle_x), 0.0f, -sin(-angle_x), 0.0f,
					0.0f, 1.0f, 0.0f, 0.0f,
					sin(-angle_x), 0.0f, cos(-angle_x), 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f
				};
				float rotate_y[16] = {
					1.0f, 0.0f, 0.0f, 0.0f,
					0.0f, cos(angle_y), -sin(angle_y), 0.0f,
					0.0f, sin(angle_y), cos(angle_y), 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f
				};
				multipleMat4f(rotate_x, rotate_y, matrix);
				view = Glsl::Mat4(matrix);
				shader.setUniform("view", view);
				Mouse::setPosition(Vector2i(resolution.x / 2, resolution.y / 2), window);
			}
		}
		window.clear();
		texture.draw(sprite, &shader);
		window.draw(sprite);
		window.display();
	}

	delete[] matrix;

#endif

	return 0;
}