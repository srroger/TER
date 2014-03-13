#include "stdafx.h"
#include "RubixCube.h"

RubixCube::RubixCube(double length)
{
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
	for (i = 0; i<27; i += 9)
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
}
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
	if (!rotating)
	{
		double disp = len + 0.2;
		glPushMatrix();
		for (int i = 0; i<27; i++)
			c[i].display();

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
		}
		glColor4f(1, 1, 1, 0.3);
		glPopMatrix();
	}
	else
		display_rotation();
}

