#ifndef RUBIXCUBE_H
#define RUBIXCUBE_H
#include <cmath>
#include "cstdlib"
#include "ctime"
#include "vector"
#include "windows.h"
#include "glut.h"
#include "Cube.h"

class RubixCube
{
	Cube c[27];
	double len, theta;
	int dir, cubepos[3][3][3];
	bool rotating;
	std::vector<int> axis, plane,
		keypressed/*0=axis change, 1=plane change, 2=clockwise rot, 3=anticlockwise rot*/;
public:
	RubixCube(double length);
	void set_centre();
	void display();
	void display_rotation();
	void rotation_idle_func();
	void keyboard(unsigned char key);
	friend void change_pos(int dir, int & a0, int & a1, int & a2, int & b0, int & b1, int & b2, int & c0, int & c1, int & c2);
	
};

#endif
