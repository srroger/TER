
#include "stdafx.h"
#include "Cube.h"
Cube::Cube()
{
	cx = cy = cz = 0;
	len = 1;
	for (int i = 0; i<6; i++)
		color[i] = 0;
}
void Cube::set_length(double length)
{
	len = length;
}
void Cube::set_color(int color)
{
	switch (color)
	{
	case 0: glColor3f(0, 0, 0); break;
	case 1: glColor3f(1, 1, 0); break;
	case 2: glColor3f(0.3, 0.5, 1); break;
	case 3: glColor3f(1, 0, 0); break;
	case 4: glColor3f(0, 0.8, 0); break;
	case 5: glColor3f(0, 0, 1); break;
	case 6: glColor3f(1, 1, 1); break;
	default: glColor3f(0, 0, 0);
	}
}
void Cube::display()
{
	glPushMatrix();
	glTranslatef(cx, cy, cz);
	/*Face 1 XY-Plane (Behind)*/
	set_color(color[0]);
	glBegin(GL_POLYGON);
	glVertex3d(-len / 2, -len / 2, -len / 2); glVertex3d(-len / 2, len / 2, -len / 2); glVertex3d(len / 2, len / 2, -len / 2); glVertex3d(len / 2, -len / 2, -len / 2);
	glEnd();
	/*Face 2 XZ-Plane (Bottom)*/
	set_color(color[1]);
	glBegin(GL_POLYGON);
	glVertex3d(-len / 2, -len / 2, -len / 2); glVertex3d(len / 2, -len / 2, -len / 2); glVertex3d(len / 2, -len / 2, len / 2); glVertex3d(-len / 2, -len / 2, len / 2);
	glEnd();
	/*Face 3 YZ-Plane (Left)*/
	set_color(color[2]);
	glBegin(GL_POLYGON);
	glVertex3d(-len / 2, -len / 2, len / 2); glVertex3d(-len / 2, len / 2, len / 2); glVertex3d(-len / 2, len / 2, -len / 2); glVertex3d(-len / 2, -len / 2, -len / 2);
	glEnd();
	/*Face 4 XY-Plane (Front)*/
	set_color(color[3]);
	glBegin(GL_POLYGON);
	glVertex3d(len / 2, -len / 2, len / 2); glVertex3d(len / 2, len / 2, len / 2); glVertex3d(-len / 2, len / 2, len / 2); glVertex3d(-len / 2, -len / 2, len / 2);
	glEnd();
	/*Face 5 XZ-Plane (Top)*/
	set_color(color[4]);
	glBegin(GL_POLYGON);
	glVertex3d(-len / 2, len / 2, len / 2); glVertex3d(len / 2, len / 2, len / 2); glVertex3d(len / 2, len / 2, -len / 2); glVertex3d(-len / 2, len / 2, -len / 2);
	glEnd();
	/*Face 6 YZ-Plane (Right)*/
	set_color(color[5]);
	glBegin(GL_POLYGON);
	glVertex3d(len / 2, -len / 2, len / 2); glVertex3d(len / 2, len / 2, len / 2); glVertex3d(len / 2, len / 2, -len / 2); glVertex3d(len / 2, -len / 2, -len / 2);
	glEnd();
	glPopMatrix();
}
void Cube::rotate(int axis, int dir)
{
	int temp;
	switch (axis)
	{
	case 0:
		if (dir)
		{
			temp = color[0]; color[0] = color[4]; color[4] = color[3]; color[3] = color[1]; color[1] = temp;
		}
		else
		{
			temp = color[0]; color[0] = color[1]; color[1] = color[3]; color[3] = color[4]; color[4] = temp;
		}
		break;
	case 1:
		if (dir)
		{
			temp = color[0]; color[0] = color[2]; color[2] = color[3]; color[3] = color[5]; color[5] = temp;
		}
		else
		{
			temp = color[0]; color[0] = color[5]; color[5] = color[3]; color[3] = color[2]; color[2] = temp;
		}
		break;
	case 2:
		if (dir)
		{
			temp = color[1]; color[1] = color[5]; color[5] = color[4]; color[4] = color[2]; color[2] = temp;
		}
		else
		{
			temp = color[1]; color[1] = color[2]; color[2] = color[4]; color[4] = color[5]; color[5] = temp;
		}
		break;
	}
}