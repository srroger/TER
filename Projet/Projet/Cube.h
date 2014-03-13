#ifndef CUBE_H
#define CUBE_H
#include <cmath>
#include "cstdlib"
#include "ctime"
#include "vector"
#include "windows.h"
#include "glut.h"

using namespace std;

class Cube
{
	double cx, cy, cz, len;
	int color[6];
public:
	Cube();
	void set_color(int color);
	void display();
	void rotate(int axis, int dir);
	void set_length(double length);
	friend class RubixCube;
};

#endif