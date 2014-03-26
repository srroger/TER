#ifndef VIEWER_H
#define VIEWER_H
#include <cmath>
#include "cstdlib"
#include "ctime"
#include "vector"
#include "windows.h"
#include "glut.h"

#include "RubixCube.h"
class Viewer
{
	double pos[3];
	double v_alpha, v_beta, v_radius, v_min_radius;

public:
	
	RubixCube* r;
	Viewer(double alpha, double beta, double radius, double min_radius,RubixCube* ru);
	void angle_range_check();
	void set_position();
	void special_keyboard(int key);
	void reset_view(double alpha, double beta, double radius, double min_radius);
	void LookAtCentre();
	void Rotation(int sens);

};

#endif