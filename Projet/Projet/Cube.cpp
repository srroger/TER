
#include "stdafx.h"
#include "Cube.h"

/**
		    ^y
		    |
		z<--x
*/

PointGL Cube::transformRotX(GLfloat val, PointGL pt)
{

	for(int i = 0 ; i < 27 ; i++)
	{
		float cxprime = pt.x;
		float cyprime = pt.y*cos(DEG_TO_RAD*val) - pt.z*sin(DEG_TO_RAD*val);
		float czprime = pt.y*sin(DEG_TO_RAD*val) + pt.z*cos(DEG_TO_RAD*val);

		pt.x = cxprime;
		pt.y = cyprime;
		pt.z = czprime;
	}
	
	return pt;
}

PointGL Cube::transformRotY(GLfloat val, PointGL pt)
{

	for(int i = 0 ; i < 27 ; i++)
	{
		float cxprime = pt.x*cos(DEG_TO_RAD*val) + pt.z*sin(DEG_TO_RAD*val);
		float cyprime = pt.y;
		float czprime = -pt.x*sin(DEG_TO_RAD*val) + pt.z*cos(DEG_TO_RAD*val);

		pt.x = cxprime;
		pt.y = cyprime;
		pt.z = czprime;

	}
	
	return pt;
}


PointGL Cube::transformRotZ(GLfloat val, PointGL pt)
{

	
	for(int i = 0 ; i < 27 ; i++)
	{
		float cxprime = pt.x*cos(DEG_TO_RAD*val) - pt.y*sin(DEG_TO_RAD*val);
		float cyprime = pt.x*sin(DEG_TO_RAD*val) + pt.y*cos(DEG_TO_RAD*val);
		float czprime = pt.z;

		pt.x = cxprime;
		pt.y = cyprime;
		pt.z = czprime;
	}
	return pt;
}

Cube::Cube()
{
	rx = 0; ry = 0; rz = 0;
	AxisX[0] = 1; AxisX[1] = 0; AxisX[2] = 0;
	AxisY[0] = 0; AxisY[1] = 1; AxisY[2] = 0;
	AxisZ[0] = 0; AxisZ[1] = 0; AxisZ[2] = 1;

	cx = cy = cz = 0;
	len = 1;
	//
	//coins[0].x =cx -len / 2; coins[0].y =  cy -len / 2; coins[0].z = cz -len / 2 ;
	//coins[1].x = cx-len / 2; coins[1].y = cy + len / 2; coins[1].z = cz -len / 2;
	//coins[2] = PointGL(cx+len / 2,cy + len / 2,cz -len / 2);
	//coins[3] = PointGL(cx+len / 2,cy  -len / 2,cz -len / 2);
	//coins[4] = PointGL(cx+len / 2,cy  -len / 2,cz+ len / 2);
	//coins[5] = PointGL(cx-len / 2,cy  -len / 2,cz+ len / 2);
	//coins[6] = PointGL(cx-len / 2,cy + len / 2,cz+ len / 2);
	//coins[7] = PointGL(cx+len / 2,cy + len / 2,cz+ len / 2);
	for (int i = 0; i<6; i++)
		color[i] = 0;
}
void Cube::set_length(double length)
{
	len = length;
		coins[0].x =-len / 2; coins[0].y =   -len / 2; coins[0].z =  -len / 2 ;
	coins[1].x = -len / 2; coins[1].y =  + len / 2; coins[1].z =  -len / 2;
	coins[2] = PointGL(+len / 2, + len / 2, -len / 2);
	coins[3] = PointGL(+len / 2,  -len / 2, -len / 2);
	coins[4] = PointGL(+len / 2, -len / 2,+ len / 2);
	coins[5] = PointGL(-len / 2,  -len / 2,+ len / 2);
	coins[6] = PointGL(-len / 2, + len / 2,+ len / 2);
	coins[7] = PointGL(+len / 2, + len / 2,+ len / 2);
}
void Cube::set_color(int color)
{
	switch (color)
	{
	//case 0: glColor3f(0, 0, 0); break; // noir
	case 4: glColor3f(0, 0.8, 0); break; // bleu
	case 2: glColor3f(1, 0, 0); break; // rouge
	case 5: glColor3f(1, 0.5, 0); break; // Jaune
	case 1: glColor3f(0, 0, 1); break; // vert
	case 3: glColor3f(1, 1, 0); break; //orange
	case 6: glColor3f(1, 1, 1); break; // blanc
	default: glColor3f(0, 0, 0); // noir
	}
}


//
//void Cube::display()
//{
//	glPushMatrix();
//	glTranslatef(cx, cy, cz);
//	//glRotatef(rx, AxisX[0], AxisX[1], AxisX[2]);
//	//glRotatef(ry, AxisY[0], AxisY[1], AxisY[2]);
//	//glRotatef(rz, AxisZ[0], AxisZ[1], AxisZ[2]);
//	
//	
//	/*Face 1 XY-Plane (Behind)*/
//	set_color(color[0]);
//	glBegin(GL_POLYGON);
//	glVertex3d(coins[0].x,coins[0].y,coins[0].z);  //0
//	glVertex3d(coins[1].x,coins[1].y,coins[1].z); //1
//	glVertex3d(coins[2].x,coins[2].y,coins[2].z);  //2 
//	glVertex3d(coins[3].x,coins[3].y,coins[3].z); //3
//	glEnd();
//
//
//	/*Face 2 XZ-Plane (Bottom)*/
//	set_color(color[1]);
//	glBegin(GL_POLYGON);
//	glVertex3d(coins[0].x,coins[0].y,coins[0].z); //0
//	glVertex3d(coins[3].x,coins[3].y,coins[3].z); //3
//	glVertex3d(coins[4].x,coins[4].y,coins[4].z);  //4
//	glVertex3d(coins[5].x,coins[5].y,coins[5].z); //5
//	glEnd();
//	/*Face 3 YZ-Plane (Left)*/
//	set_color(color[2]);
//	glBegin(GL_POLYGON);
//	glVertex3d(coins[5].x,coins[5].y,coins[5].z); //5
//	glVertex3d(coins[6].x,coins[6].y,coins[6].z);  //6
//	glVertex3d(coins[1].x,coins[1].y,coins[1].z); // 1
//	glVertex3d(coins[0].x,coins[0].y,coins[0].z);// 0
//	glEnd();
//	/*Face 4 XY-Plane (Front)*/
//	set_color(color[3]);
//	glBegin(GL_POLYGON);
//	glVertex3d(coins[4].x,coins[4].y,coins[4].z); //4
//	glVertex3d(coins[7].x,coins[7].y,coins[7].z);  //7
//	glVertex3d(coins[6].x,coins[6].y,coins[6].z);  //6
//	glVertex3d(coins[5].x,coins[5].y,coins[5].z); //5
//	glEnd();
//	/*Face 5 XZ-Plane (Top)*/
//	set_color(color[4]);
//	glBegin(GL_POLYGON);
//	glVertex3d(coins[6].x,coins[6].y,coins[6].z); //6
//	glVertex3d(coins[7].x,coins[7].y,coins[7].z); //7
//	glVertex3d(coins[2].x,coins[2].y,coins[2].z); //2
//	glVertex3d(coins[1].x,coins[1].y,coins[1].z);//1
//	glEnd();
//	/*Face 6 YZ-Plane (Right)*/
//	set_color(color[5]);
//	glBegin(GL_POLYGON);
//	glVertex3d(coins[4].x,coins[4].y,coins[4].z); //4
//	glVertex3d(coins[7].x,coins[7].y,coins[7].z); //7
//	glVertex3d(coins[2].x,coins[2].y,coins[2].z); //2
//	glVertex3d(coins[3].x,coins[3].y,coins[3].z); //3
//	glEnd();
//	glPopMatrix();
//}


void Cube::display()
{
	glPushMatrix();
	glTranslatef(cx, cy, cz);
	glRotatef(rx, AxisX[0], AxisX[1], AxisX[2]);
	glRotatef(ry, AxisY[0], AxisY[1], AxisY[2]);
	glRotatef(rz, AxisZ[0], AxisZ[1], AxisZ[2]);
	
	
	/*Face 1 XY-Plane (Behind)*/
	set_color(color[0]);
	glBegin(GL_POLYGON);
	glVertex3d(-len / 2, -len / 2, -len / 2); 
	glVertex3d(-len / 2, len / 2, -len / 2); 
	glVertex3d(len / 2, len / 2, -len / 2); 
	glVertex3d(len / 2, -len / 2, -len / 2);
	glEnd();


	/*Face 2 XZ-Plane (Bottom)*/
	set_color(color[1]);
	glBegin(GL_POLYGON);
	glVertex3d(-len / 2, -len / 2, -len / 2); 
	glVertex3d(len / 2, -len / 2, -len / 2); 
	glVertex3d(len / 2, -len / 2, len / 2); 
	glVertex3d(-len / 2, -len / 2, len / 2);
	glEnd();
	/*Face 3 YZ-Plane (Left)*/
	set_color(color[2]);
	glBegin(GL_POLYGON);
	glVertex3d(-len / 2, -len / 2, len / 2); 
	glVertex3d(-len / 2, len / 2, len / 2); 
	glVertex3d(-len / 2, len / 2, -len / 2); 
	glVertex3d(-len / 2, -len / 2, -len / 2);
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

	/**
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
	}*/
}

void Cube::MoveRX(GLfloat val)
{
	for(int i = 0 ; i<8 ; i++)
	{
		coins[i] = transformRotX(val,coins[i]);
	}
}

void Cube::MoveRY(GLfloat val)
{
	for(int i = 0 ; i<8 ; i++)
	{
		coins[i] = transformRotY(val,coins[i]);
	}
}

void Cube::MoveRZ(GLfloat val)
{
	for(int i = 0 ; i<8 ; i++)
	{
		coins[i] = transformRotZ(val,coins[i]);
	}
}
