#ifndef CUBE_H
#define CUBE_H
#include <cmath>
#include "cstdlib"
#include "ctime"
#include "vector"
#include "windows.h"
#include "glut.h"

using namespace std;

#ifndef M_PI
// Source: http://www.geom.uiuc.edu/~huberty/math5337/groupe/digits.html
#define M_PI 3.141592653589793238462643383279502884197169399375105820974944592307816406 
#endif
#define DEG_TO_RAD (M_PI/180.0) //multiplier cette valeur par le degree
#define RAD_TO_DEG (180.0/M_PI)

struct PointGL{
	float x,y,z;
	int couleurFacile;
	PointGL()
	{

	}
	PointGL(float xp, float yp, float zp)
	{
		x=xp;
		y=yp;
		z=zp;
	}
};
struct PointPolaire{
	float r,teta;
	int couleurFacile;
};

class Cube
{
	public:
	double cx, cy, cz, len;
	PointGL coins[8];

	int color[6];
	
	GLfloat rx,ry,rz; // L'angle de rotation.
	
	float AxisX[3];
	float AxisY[3];
	float AxisZ[3];
	
	int type; //0: centre, 1 : coin, 2 : bord

	Cube();
	void set_color(int color);
	void display();
	void rotate(int axis, int dir);
	void set_length(double length);
	

	PointGL transformRotX(GLfloat val, PointGL pt);
	PointGL transformRotY(GLfloat val, PointGL pt);
	PointGL transformRotZ(GLfloat val, PointGL pt);
	void MoveRX(GLfloat val);
	void MoveRY(GLfloat val);
	void MoveRZ(GLfloat val);

	friend class RubixCube;
};

#endif