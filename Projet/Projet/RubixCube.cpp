#include "stdafx.h"
#include "RubixCube.h"
/**
		    ^y
		    |
		z<--x
*/

#ifndef M_PI
// Source: http://www.geom.uiuc.edu/~huberty/math5337/groupe/digits.html
#define M_PI 3.141592653589793238462643383279502884197169399375105820974944592307816406 
#endif
#define DEG_TO_RAD (M_PI/180.0) //multiplier cette valeur par le degree
#define RAD_TO_DEG (180.0/M_PI)

RubixCube::RubixCube(double length)
{
	x= 0; y=0 ; z=0;
	rx = 0; ry = 30; rz = 0;
	
	AxisX[0] = 1; AxisX[1] = 0; AxisX[2] = 0;
	AxisY[0] = 0; AxisY[1] = 1; AxisY[2] = 0;
	AxisZ[0] = 0; AxisZ[1] = 0; AxisZ[2] = 1;
	
	len = length;
	plane.push_back(2);
	axis.push_back(0);
	rotating = false;
	theta = 0;
	dir = 0;
	int p = 0, i, j, k;
	for (i = 0; i<27; i++)
		c[i].set_length(len);

	for (i = 0; i<9; i++)
	{
		c[i].color[2] = 3; c[i + 18].color[5] = 6;
	}
	for (i = 0; i<27; i += 9) // color=face d'un cube
	{
		c[i + 5].color[0] = 1; c[i + 8].color[0] = 1; c[i + 2].color[0] = 1;
		c[i + 2].color[1] = 2; c[i + 1].color[1] = 2; c[i].color[1] = 2;
		c[i].color[3] = 4; c[i + 3].color[3] = 4; c[i + 6].color[3] = 4;
		c[i + 6].color[4] = 5; c[i + 7].color[4] = 5; c[i + 8].color[4] = 5;
	}
	for (i = 0; i<3; i++)
	for (j = 0; j<3; j++)
	for (k = 0; k<3; k++)
		cubepos[i][j][k] = p++;
	set_centre();
}

/**
void RubixCube::display_rotation()
{
	int i, j, k;
	double angle, disp = len + 0.2;

	if (dir) angle = -theta;
	else angle = theta;

	glPushMatrix();
	switch (axis.back())
	{
	case 0:
		for (i = 0; i<3; i++)
		{
			if (i != plane.back())
			{
				for (j = 0; j<3; j++)
				for (k = 0; k<3; k++)
					c[cubepos[i][j][k]].display();
			}
		}
		glRotated(angle, 1, 0, 0);
		for (j = 0; j<3; j++)
		for (k = 0; k<3; k++)
			c[cubepos[plane.back()][j][k]].display();
		break;
	case 1:
		for (j = 0; j<3; j++)
		{
			if (j != plane.back())
			{
				for (i = 0; i<3; i++)
				for (k = 0; k<3; k++)
					c[cubepos[i][j][k]].display();
			}
		}
		glRotated(angle, 0, 1, 0);
		for (i = 0; i<3; i++)
		for (k = 0; k<3; k++)
			c[cubepos[i][plane.back()][k]].display();
		break;
	case 2:
		for (k = 0; k<3; k++)
		{
			if (k != plane.back())
			{
				for (i = 0; i<3; i++)
				for (j = 0; j<3; j++)
					c[cubepos[i][j][k]].display();
			}
		}
		glRotated(angle, 0, 0, 1);
		for (i = 0; i<3; i++)
		for (j = 0; j<3; j++)
			c[cubepos[i][j][plane.back()]].display();
		break;
	}
	if (axis.back() == 0)
	{
		if (plane.back() == 0) glTranslated(-disp, 0, 0);
		else if (plane.back() == 2) glTranslated(disp, 0, 0);
		glRotatef(90, 0, 1, 0);
	}
	else if (axis.back() == 1)
	{
		if (plane.back() == 0) glTranslated(0, -disp, 0);
		else if (plane.back() == 2) glTranslated(0, disp, 0);
		glRotatef(90, 1, 0, 0);
	}
	else
	{
		if (plane.back() == 0) glTranslated(0, 0, disp);
		else if (plane.back() == 2) glTranslated(0, 0, -disp);
	}
	glColor4f(1, 1, 1, 0.3);
	glRectd(-(len*2.25), -(len*2.25), (len*2.25), (len*2.25));
	glPopMatrix();
}
void RubixCube::rotation_idle_func()
{
	if (rotating)
	{
		if (theta >= (double)90)
		{
			glutIdleFunc(NULL);
			rotating = false;
			theta = 0.0;
		}
		else
			theta += +0.5;
		    glutPostRedisplay();
	}
}*/
void RubixCube::set_centre()
{
	int i, j, k;
	double dist = len + 0.2;
	for (j = 0; j<3; j++)
	{
		for (k = 0; k<3; k++)
		{
			c[cubepos[0][j][k]].cx = -dist;
			c[cubepos[1][j][k]].cx = 0;
			c[cubepos[2][j][k]].cx = dist;
		}
	}
	for (i = 0; i<3; i++)
	{
		for (j = 0; j<3; j++)
		{
			c[cubepos[i][j][0]].cz = dist;
			c[cubepos[i][j][1]].cz = 0;
			c[cubepos[i][j][2]].cz = -dist;
		}
	}
	for (i = 0; i<3; i++)
	{
		for (k = 0; k<3; k++)
		{
			c[cubepos[i][0][k]].cy = -dist;
			c[cubepos[i][1][k]].cy = 0;
			c[cubepos[i][2][k]].cy = dist;
		}
	}
}
void RubixCube::display()
{
		double disp = len + 0.2;
		//Profondeur,Hauteur,Lateral
		/**
		    ^y
		    |
		z<--x
		*/
		glPushMatrix(); glTranslatef(x,y,z);
		//Effectue les rotations
		glRotatef(rx, AxisX[0], AxisX[1], AxisX[2]);
		glRotatef(ry, AxisY[0], AxisY[1], AxisY[2]);
		glRotatef(rz, AxisZ[0], AxisZ[1], AxisZ[2]);
		

		for (int i = 0; i<27; i++)
			c[i].display();

		/**
		if (axis.back() == 0)
		{
			if (plane.back() == 0) glTranslated(-disp, 0, 0);
			else if (plane.back() == 2) glTranslated(disp, 0, 0);
			glRotatef(90, 0, 1, 0);
		}
		else if (axis.back() == 1)
		{
			if (plane.back() == 0) glTranslated(0, -disp, 0);
			else if (plane.back() == 2) glTranslated(0, disp, 0);
			glRotatef(90, 1, 0, 0);
		}
		else
		{
			if (plane.back() == 0) glTranslated(0, 0, len);
			else if (plane.back() == 2) glTranslated(0, 0, -len);
		}		*/
		glColor4f(1, 1, 1, 0.3);
		glPopMatrix();
}


void RubixCube::moveZ(GLfloat val)
{
	z = z+val;
}

void RubixCube::moveY(GLfloat val)
{
	y = y + val; 
}
void RubixCube::moveX(GLfloat val)
{
	x = x + val; 
}

void RubixCube::moveRX(GLfloat val)
{
	rx = rx + val; if(rx > 360) rx= rx-360; if(rx < -360) rx= rx+360;
	
	////Rotation Ox
	//AxisX[0] = AxisX[0] - (AxisX[0] - AxisX[0]);
	//AxisX[1] = AxisX[1] - (-AxisX[2]*sin(DEG_TO_RAD*val) + AxisX[1]*cos(DEG_TO_RAD*val) - AxisX[1]);
	//AxisX[2] = AxisX[2] - (AxisX[2]*cos(DEG_TO_RAD*val) +  AxisX[1]*sin(DEG_TO_RAD*val) - AxisX[2]);
	//
	////Rotation Ox
	//AxisY[0] = AxisY[0] - (AxisY[0] - AxisY[0]);
	//AxisY[1] = AxisY[1] - (-AxisY[2]*sin(DEG_TO_RAD*val) + AxisY[1]*cos(DEG_TO_RAD*val) - AxisY[1]);
	//AxisY[2] = AxisY[2] - (AxisY[2]*cos(DEG_TO_RAD*val) +  AxisY[1]*sin(DEG_TO_RAD*val) - AxisY[2]);

	////Rotation Ox
	//AxisZ[0] = AxisZ[0] - (AxisZ[0] - AxisZ[0]);
	//AxisZ[1] = AxisZ[1] - (-AxisZ[2]*sin(DEG_TO_RAD*val) + AxisZ[1]*cos(DEG_TO_RAD*val) - AxisZ[1]);
	//AxisZ[2] = AxisZ[2] - (AxisZ[2]*cos(DEG_TO_RAD*val) +  AxisZ[1]*sin(DEG_TO_RAD*val) - AxisZ[2]);

	
}
void RubixCube::moveRY(GLfloat val)
{
	ry = ry + val; if(ry > 360) ry= ry-360; if(ry < -360) ry= ry+360;
	
	////Rotation Oy
	//AxisX[0] = AxisX[0] - (AxisX[0]*cos(DEG_TO_RAD*val) + AxisX[2]*sin(DEG_TO_RAD*val) - AxisX[0]);
	//AxisX[1] = AxisX[1] - (AxisX[1] - AxisX[1]);
	//AxisX[2] = AxisX[2] - (-AxisX[0]*sin(DEG_TO_RAD*val) +  AxisX[2]*cos(DEG_TO_RAD*val) - AxisX[2]);
	//
	////Rotation Oy
	//AxisY[0] = AxisY[0] - (AxisY[0]*cos(DEG_TO_RAD*val) + AxisY[2]*sin(DEG_TO_RAD*val) - AxisY[0]);
	//AxisY[1] = AxisY[1] - (AxisY[1] - AxisY[1]);
	//AxisY[2] = AxisY[2] - (-AxisY[0]*sin(DEG_TO_RAD*val) +  AxisY[2]*cos(DEG_TO_RAD*val) - AxisY[2]);

	////Rotation Oy
	//AxisZ[0] = AxisZ[0] - (AxisZ[0]*cos(DEG_TO_RAD*val) + AxisZ[2]*sin(DEG_TO_RAD*val) - AxisZ[0]);
	//AxisZ[1] = AxisZ[1] - (AxisZ[1] - AxisZ[1]);
	//AxisZ[2] = AxisZ[2] - (-AxisZ[0]*sin(DEG_TO_RAD*val) +  AxisZ[2]*cos(DEG_TO_RAD*val) - AxisZ[2]);

	
}
void RubixCube::moveRZ(GLfloat val)
{
	rz = rz + val; if(rz > 360) rz= rz-360; if(rz < -360) rz= rz+360;
	
	////Rotation Oz
	//AxisX[0] = AxisX[0] - (AxisX[0]*cos(DEG_TO_RAD*val) + AxisX[1]*sin(DEG_TO_RAD*val) - AxisX[0]);
	//AxisX[1] = AxisX[1] - (-AxisX[0]*sin(DEG_TO_RAD*val) + AxisX[1]*cos(DEG_TO_RAD*val) - AxisX[1]);
	//AxisX[2] = AxisX[2] - (AxisX[2] - AxisX[2]);

	////Rotation Oz
	//AxisY[0] = AxisY[0] - (AxisY[0]*cos(DEG_TO_RAD*val) + AxisY[1]*sin(DEG_TO_RAD*val) - AxisY[0]);
	//AxisY[1] = AxisY[1] - (-AxisY[0]*sin(DEG_TO_RAD*val) + AxisY[1]*cos(DEG_TO_RAD*val) - AxisY[1]);
	//AxisY[2] = AxisY[2] - (AxisY[2] - AxisY[2]);

	////Rotation Oz
	//AxisZ[0] = AxisZ[0] - (AxisZ[0]*cos(DEG_TO_RAD*val) + AxisZ[1]*sin(DEG_TO_RAD*val) - AxisZ[0]);
	//AxisZ[1] = AxisZ[1] - (-AxisZ[0]*sin(DEG_TO_RAD*val) + AxisZ[1]*cos(DEG_TO_RAD*val) - AxisZ[1]);
	//AxisZ[2] = AxisZ[2] - (AxisZ[2] - AxisZ[2]);

}

void RubixCube::reinit()
{
	rx = 0;
	ry = 0;
	rz = 0;
	x = 0;
	y = 0;
	z = 0; 
}


void RubixCube::recalcAxis()
{
	AxisX[0] = 1; AxisX[1] = 0; AxisX[2] = 0;
	AxisY[0] = 0; AxisY[1] = 1; AxisY[2] = 0;
	AxisZ[0] = 0; AxisZ[1] = 0; AxisZ[2] = 1;

	/**
	//Rotation Ox
	AxisX[0] = AxisX[0] - (AxisX[0] - AxisX[0]);
	AxisX[1] = AxisX[1] - (-AxisX[2]*sin(DEG_TO_RAD*rx) + AxisX[1]*cos(DEG_TO_RAD*rx) - AxisX[1]);
	AxisX[2] = AxisX[2] - (AxisX[2]*cos(DEG_TO_RAD*rx) +  AxisX[1]*sin(DEG_TO_RAD*rx) - AxisX[2]);
	
	//Rotation Oy
	AxisX[0] = AxisX[0] - (AxisX[0]*cos(DEG_TO_RAD*ry) + AxisX[2]*sin(DEG_TO_RAD*ry) - AxisX[0]);
	AxisX[1] = AxisX[1] - (AxisX[1] - AxisX[1]);
	AxisX[2] = AxisX[2] - (-AxisX[0]*sin(DEG_TO_RAD*ry) +  AxisX[2]*cos(DEG_TO_RAD*ry) - AxisX[2]);
	
	//Rotation Oz
	AxisX[0] = AxisX[0] - (AxisX[0]*cos(DEG_TO_RAD*rz) + AxisX[1]*sin(DEG_TO_RAD*rz) - AxisX[0]);
	AxisX[1] = AxisX[1] - (-AxisX[0]*sin(DEG_TO_RAD*rz) + AxisX[1]*cos(DEG_TO_RAD*rz) - AxisX[1]);
	AxisX[2] = AxisX[2] - (AxisX[2] - AxisX[2]);


	//Rotation Ox
	AxisY[0] = AxisY[0] - (AxisY[0] - AxisY[0]);
	AxisY[1] = AxisY[1] - (-AxisY[2]*sin(DEG_TO_RAD*rx) + AxisY[1]*cos(DEG_TO_RAD*rx) - AxisY[1]);
	AxisY[2] = AxisY[2] - (AxisY[2]*cos(DEG_TO_RAD*rx) +  AxisY[1]*sin(DEG_TO_RAD*rx) - AxisY[2]);
	
	//Rotation Oy
	AxisY[0] = AxisY[0] - (AxisY[0]*cos(DEG_TO_RAD*ry) + AxisY[2]*sin(DEG_TO_RAD*ry) - AxisY[0]);
	AxisY[1] = AxisY[1] - (AxisY[1] - AxisY[1]);
	AxisY[2] = AxisY[2] - (-AxisY[0]*sin(DEG_TO_RAD*ry) +  AxisY[2]*cos(DEG_TO_RAD*ry) - AxisY[2]);

	//Rotation Oz
	AxisY[0] = AxisY[0] - (AxisY[0]*cos(DEG_TO_RAD*rz) + AxisY[1]*sin(DEG_TO_RAD*rz) - AxisY[0]);
	AxisY[1] = AxisY[1] - (-AxisY[0]*sin(DEG_TO_RAD*rz) + AxisY[1]*cos(DEG_TO_RAD*rz) - AxisY[1]);
	AxisY[2] = AxisY[2] - (AxisY[2] - AxisY[2]);


	//Rotation Ox
	AxisZ[0] = AxisZ[0] - (AxisZ[0] - AxisZ[0]);
	AxisZ[1] = AxisZ[1] - (-AxisZ[2]*sin(DEG_TO_RAD*rx) + AxisZ[1]*cos(DEG_TO_RAD*rx) - AxisZ[1]);
	AxisZ[2] = AxisZ[2] - (AxisZ[2]*cos(DEG_TO_RAD*rx) +  AxisZ[1]*sin(DEG_TO_RAD*rx) - AxisZ[2]);

	//Rotation Oy
	AxisZ[0] = AxisZ[0] - (AxisZ[0]*cos(DEG_TO_RAD*ry) + AxisZ[2]*sin(DEG_TO_RAD*ry) - AxisZ[0]);
	AxisZ[1] = AxisZ[1] - (AxisZ[1] - AxisZ[1]);
	AxisZ[2] = AxisZ[2] - (-AxisZ[0]*sin(DEG_TO_RAD*ry) +  AxisZ[2]*cos(DEG_TO_RAD*ry) - AxisZ[2]);

	//Rotation Oz
	AxisZ[0] = AxisZ[0] - (AxisZ[0]*cos(DEG_TO_RAD*rz) + AxisZ[1]*sin(DEG_TO_RAD*rz) - AxisZ[0]);
	AxisZ[1] = AxisZ[1] - (-AxisZ[0]*sin(DEG_TO_RAD*rz) + AxisZ[1]*cos(DEG_TO_RAD*rz) - AxisZ[1]);
	AxisZ[2] = AxisZ[2] - (AxisZ[2] - AxisZ[2]);


	
	*/
}
