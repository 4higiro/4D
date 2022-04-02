// Including
#include <iostream>
#include <glad/glad.h>

#include "tesseract.h"

// Using names
using namespace std;

// Point construct
tesseract::point::point()
{
	coords[0] = 0;
	coords[1] = 0;
	coords[2] = 0;

	is_real = false;
}

// Line construct
tesseract::line::line()
{
	for (int i = 0; i < 4; i++)
	{
		this->m1[i] = 0;
		this->m2[i] = 0;
		p[i] = 0;
	}
}

// Update of navigation vector
void tesseract::line::update()
{
	for (int i = 0; i < 4; i++)
	{
		p[i] = m2[i] - m1[i];
	}
}

// Sample line;
tesseract::point tesseract::line::sample(float t)
{
	point result;

	result.coords[0] = m1[0] + t * p[0];
	result.coords[1] = m1[1] + t * p[1];
	result.coords[2] = m1[2] + t * p[2];

	return result;
}

// Tesseract construct
tesseract::tesseract()
{
	int q_verticies[8][4] = {
		{ 0, 0, 0, 0 },
		{ 1, 1, 0, 0 },
		{ 0, 1, 1, 0 },
		{ 1, 0, 1, 0 },
		{ 1, 1, 1, 1 },
		{ 1, 0, 0, 1 },
		{ 0, 0, 1, 1 },
		{ 0, 1, 0, 1 }
	};

	int count = 0;

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			lines[count].m1[0] = q_verticies[i][0];
			lines[count].m1[1] = q_verticies[i][1];
			lines[count].m1[2] = q_verticies[i][2];
			lines[count].m1[3] = q_verticies[i][3];

			if (j == 0)
				lines[count].m2[0] = 1 - lines[count].m1[0];
			else
				lines[count].m2[0] = lines[count].m1[0];
			
			if (j == 1)
				lines[count].m2[1] = 1 - lines[count].m1[1];
			else
				lines[count].m2[1] = lines[count].m1[1];

			if (j == 2)
				lines[count].m2[2] = 1 - lines[count].m1[2];
			else
				lines[count].m2[2] = lines[count].m1[2];

			if (j == 03)
				lines[count].m2[3] = 1 - lines[count].m1[3];
			else
				lines[count].m2[3] = lines[count].m1[3];

			lines[count].update();

			count++;
		}
	}
}

// Print 4D lines
void tesseract::print4D()
{
	cout << "Sections of lines:" << endl;

	for (int i = 0; i < 32; i++)
	{
		cout << "Line " << i + 1 << ":";
		cout << lines[i].m1[0] << lines[i].m1[1] << lines[i].m1[2] << lines[i].m1[3] << " ::: ";
		cout << lines[i].m2[0] << lines[i].m2[1] << lines[i].m2[2] << lines[i].m2[3] << endl;
	}
}

// Print 3D points
void tesseract::print3D()
{
	cout << "Intersections 3D-point from 4D-lines:" << endl;

	for (int i = 0; i < 64; i++)
	{
		if (points[i].is_real)
		{
			cout << "Point " << i + 1 << " : ";
			cout << points[i].coords[0] << "  " << points[i].coords[1] << "  " << points[i].coords[2] << endl;
		}
	}
}

// Sampling all lines;
void tesseract::sampling(float value)
{
	int count = 32;

	for (int i = 0; i < 32; i++)
	{
		float t = -1;

		if (lines[i].p[3] != 0)
		{
			t = (value - lines[i].m1[3]) / lines[i].p[3];
		}

		if (lines[i].m1[3] == value && lines[i].m2[3] == value)
		{
			points[count] = lines[i].sample(0);
			points[count + 1] = lines[i].sample(1);

			points[count].is_real = true;
			points[count + 1].is_real = false;

			count++;
			count++;
		}
		
		if (t >= 0 && t <= 1)
		{
			points[i] = lines[i].sample(t);
			points[i].is_real = true;
		}
		else
		{
			points[i].is_real = false;
		}
	}
}
