// Protection for repeat including
#pragma once

// Rotate axis enum type
enum axis
{
	X, Y, Z, Q
};

// Tesseract class
class tesseract
{
private:
	// Vertex of tesseract in 3D
	class point
	{
	public:
		bool is_real;
		float coords[3];

		point();
	};

	// Edge of tesseract in 4D
	class line
	{
	public:
		float p[4];
		float m1[4], m2[4];

		line();
		void update();
		point sample(float t);
	};

	line lines[32];
	point points[64];
public:
	tesseract();
	void print4D();
	void print3D();
	void rotate(axis a, float angle);
	void sampling(float value);
	void drawing();
};

