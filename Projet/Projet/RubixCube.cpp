#include "stdafx.h"
#include "RubixCube.h"
/**
		    ^y
		    |
		z<--x
*/



RubixCube::RubixCube(double length)
{
	x= 25; y=0 ; z=0;
	rx = 0; ry = 10; rz = 20;
	
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

	//TEST : 
	//c[cubepos[0][0][0]].rotate(0,0);
	/**
	for(int cpt = 0 ; cpt < 6 ; cpt++)
		cout << c[cubepos[0][1][1]].color[cpt]<<" , "; cout <<endl;
	for(int cpt = 0 ; cpt < 6 ; cpt++)
		cout << c[cubepos[2][1][1]].color[cpt]<<" , ";cout <<endl;
	for(int cpt = 0 ; cpt < 6 ; cpt++)
		cout << c[cubepos[1][0][1]].color[cpt]<<" , ";cout <<endl;
	for(int cpt = 0 ; cpt < 6 ; cpt++)
		cout << c[cubepos[1][2][1]].color[cpt]<<" , ";cout <<endl;
	for(int cpt = 0 ; cpt < 6 ; cpt++)
		cout << c[cubepos[1][1][0]].color[cpt]<<" , ";cout <<endl;
	for(int cpt = 0 ; cpt < 6 ; cpt++)
		cout << c[cubepos[1][1][2]].color[cpt]<<" , ";cout <<endl;*/
}


void RubixCube::rotation_Test()
{
	int i, j, k;
	double angle, disp = len + 0.2;

	angle = -90;

	glPushMatrix();

	for (i = 0; i<3; i++)
	{
		if (i != 1)
		{
			for (j = 0; j<3; j++)
			for (k = 0; k<3; k++)
				c[cubepos[i][j][k]].display();
		}
	}
	glRotated(angle, 1, 0, 0);
	for (j = 0; j<3; j++)
	for (k = 0; k<3; k++)
		c[cubepos[0][j][k]].display();



	glRotatef(90, 0, 1, 0);
	
	glColor4f(1, 1, 1, 0.3);
	glRectd(-(len*2.25), -(len*2.25), (len*2.25), (len*2.25));
	glPopMatrix();
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
		double disp = len + 0.2;
		//Profondeur,Hauteur,Lateral
		/**
		    ^y
		    |
		z<--x
		*/
		glPushMatrix(); 
		
		glTranslatef(x,y,z);
		//Effectue les rotations
		//glRotatef(rx, AxisX[0], AxisX[1], AxisX[2]);
		//glRotatef(ry, AxisY[0], AxisY[1], AxisY[2]);
		//glRotatef(rz, AxisZ[0], AxisZ[1], AxisZ[2]);
		//On utilise nos propres fonction de rotations, afin que les axes reste les même et que ce soit les bonne rotations. Par conséquent, glRotatef est obsolète car il modifie tout le repère en entier.
		//Maintenant pour faire une rotation, on change cx,cy,cz en fonction de la formule de rotations, et quand on fait les glVertex3d, on applique, aussi ces formules avec le rx, ry, rz globazle. Mais pour la rotation des centre on le n'utilise plus rx mais l'angle en plus tout simplement.
		//fONCTION QUI SE TROUVE DANS LES MOVErx ...


		glBegin(GL_LINES);
		glColor3f(1, 0, 0);
		glVertex3f(-50, -2, -20);
		glVertex3f(50, -2, -20);

		glColor3f(0, 1, 0);
		glVertex3f(0, -50, -20);
		glVertex3f(0, 50, -20);

		glColor3f(0, 0, 1);
		glVertex3f(0, -2, -100);
		glVertex3f(0, -2, 10);
		glEnd();

		for (int i = 0; i<27; i++)
			c[i].display();

		/**
		if (axis.back() == 0)
		{
			if (plane.bac() == 0) glTranslated(-disp, 0, 0);
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
	
	for(int i = 0 ; i < 27 ; i++)
	{
		float cxprime = c[i].cx;
		float cyprime = c[i].cy*cos(DEG_TO_RAD*val) - c[i].cz*sin(DEG_TO_RAD*val);
		float czprime = c[i].cy*sin(DEG_TO_RAD*val) + c[i].cz*cos(DEG_TO_RAD*val);

		c[i].cx = cxprime;
		c[i].cy = cyprime;
		c[i].cz = czprime;
		c[i].rx = rx;
		c[i].MoveRX(val);
	}

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
	
	for(int i = 0 ; i < 27 ; i++)
	{
		float cxprime = c[i].cx*cos(DEG_TO_RAD*val) + c[i].cz*sin(DEG_TO_RAD*val);
		float cyprime = c[i].cy;
		float czprime = -c[i].cx*sin(DEG_TO_RAD*val) + c[i].cz*cos(DEG_TO_RAD*val);

		c[i].cx = cxprime;
		c[i].cy = cyprime;
		c[i].cz = czprime;
		c[i].ry = ry;
		
		c[i].MoveRY(val);
	}




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
	
	
	for(int i = 0 ; i < 27 ; i++)
	{
		float cxprime = c[i].cx*cos(DEG_TO_RAD*val) - c[i].cy*sin(DEG_TO_RAD*val);
		float cyprime = c[i].cx*sin(DEG_TO_RAD*val) + c[i].cy*cos(DEG_TO_RAD*val);
		float czprime = c[i].cz;

		c[i].cx = cxprime;
		c[i].cy = cyprime;
		c[i].cz = czprime;
		c[i].rz = rz;
		c[i].MoveRZ(val);
	}

	
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



//Choix arbitraire (cubepos[i][j][k])
//Face 1 : cubepos[0][x][y]
//Face 2 : cubepos[2][x][y]
//Face 3 : cubepos[x][0][y]
//Face 4 : cubepos[x][2][y]
//Face 5 : cubepos[x][y][0]
//Face 6 : cubepos[x][y][0]
/*
Fonctionnement des faces et des couleurs
Chaque faces est représenter par sa couleur qui va de 1 à 6.

c[cubepos[0][1][1]].color[cpt] : 3 Orange
c[cubepos[2][1][1]].color[cpt] : 6 Blanc
c[cubepos[1][0][1]].color[cpt] : 2 Rouge
c[cubepos[1][2][1]].color[cpt] : 5 Jaune
c[cubepos[1][1][0]].color[cpt] : 4 Bleu
c[cubepos[1][1][2]].color[cpt] : 1 Vert
*/

void RubixCube::rotation_face(int face, int direction)
{
	Cube tabCub[3][3];

	switch (face)
	{
	case 1 : 
		for(int  i = 0 ; i<3;i++)
			for(int  j =  0 ; j < 3 ; j++)
			{
				tabCub[i][j] = c[cubepos[0][i][j]];
			}
		break;
	case 2 :
		for(int  i = 0 ; i<3;i++)
			for(int  j =  0 ; j < 3 ; j++)
			{
				tabCub[i][j] = c[cubepos[2][i][j]];
			}
		break;
	case 3 :
		for(int  i = 0 ; i<3;i++)
			for(int  j =  0 ; j < 3 ; j++)
			{
				tabCub[i][j] = c[cubepos[i][0][j]];
			}
		break;
	case 4 :
		for(int  i = 0 ; i<3;i++)
			for(int  j =  0 ; j < 3 ; j++)
			{
				tabCub[i][j] = c[cubepos[i][2][j]];
			}
		break;
	case 5 : 
		for(int  i = 0 ; i<3;i++)
			for(int  j =  0 ; j < 3 ; j++)
			{
				tabCub[i][j] = c[cubepos[i][j][0]];
			}
		break;
	case 6 : 
		for(int  i = 0 ; i<3;i++)
			for(int  j =  0 ; j < 3 ; j++)
			{
				tabCub[i][j] = c[cubepos[i][j][2]];
			}
		break;
	}

	//Le point centre est celui dont i=1, j=1


	//TODO





	/**

				//Calcul du barycentre C
			C.x = (ptCar[0].x + ptCar[1].x + ptCar[2].x)/3;
			C.y = (ptCar[0].y + ptCar[1].y + ptCar[2].y)/3;
			C2.x = (ptCar[0+3].x + ptCar[1+3].x + ptCar[2+3].x)/3;
			C2.y = (ptCar[0+3].y + ptCar[1+3].y + ptCar[2+3].y)/3;

			//Place les points en polaire avec pour repaire de centre C.

			ptPol[0].r = sqrt( pow((ptCar[0].x-C.x),2) + pow((ptCar[0].y - C.y),2) );
			ptPol[1].r = sqrt( pow((ptCar[1].x-C.x),2) + pow((ptCar[1].y - C.y),2) );
			ptPol[2].r = sqrt( pow((ptCar[2].x-C.x),2) + pow((ptCar[2].y - C.y),2) );
			ptPol[3].r = sqrt( pow(( ptCar[0+3].x-C2.x),2) + pow(( ptCar[0+3].y - C2.y),2) );
			ptPol[4].r = sqrt( pow(( ptCar[1+3].x-C2.x),2) + pow(( ptCar[1+3].y - C2.y),2) );
			ptPol[5].r = sqrt( pow(( ptCar[2+3].x-C2.x),2) + pow(( ptCar[2+3].y - C2.y),2) );

			ptPol[0].teta = acos( ( ptCar[0].x-C.x) / ptPol[0].r);
			ptPol[1].teta = acos( (ptCar[1].x-C.x) / ptPol[1].r);
			ptPol[2].teta = acos( (ptCar[2].x-C.x) / ptPol[2].r);
			ptPol[3].teta = acos( ( ptCar[0+3].x-C2.x) / ptPol[3].r);
			ptPol[4].teta = acos( ( ptCar[1+3].x-C2.x) / ptPol[4].r);
			ptPol[5].teta = acos( ( ptCar[2+3].x-C2.x) / ptPol[5].r);

			Calcul du barycentre C
			C.x = (ptCar[0].x + ptCar[1].x + ptCar[2].x)/3;
			C.y = (ptCar[0].y + ptCar[1].y + ptCar[2].y)/3;
			C2.x = (ptCar[0+3].x + ptCar[1+3].x + ptCar[2+3].x)/3;
			C2.y = (ptCar[0+3].y + ptCar[1+3].y + ptCar[2+3].y)/3;
			PointGL Cmoy;
			Cmoy.x = C.x + (C2.x-C.x);
			Cmoy.y = C.y + (C2.y-C.y);
			Cmoy = C;
			Place les points en polaire avec pour repaire de centre C.

			ptPol[0].r = sqrt( pow((ptCar[0].x-Cmoy.x),2) + pow((ptCar[0].y - Cmoy.y),2) );
			ptPol[1].r = sqrt( pow((ptCar[1].x-Cmoy.x),2) + pow((ptCar[1].y - Cmoy.y),2) );
			ptPol[2].r = sqrt( pow((ptCar[2].x-Cmoy.x),2) + pow((ptCar[2].y - Cmoy.y),2) );
			ptPol[3].r = sqrt( pow(( ptCar[0+3].x-Cmoy.x),2) + pow(( ptCar[0+3].y - Cmoy.y),2) );
			ptPol[4].r = sqrt( pow(( ptCar[1+3].x-Cmoy.x),2) + pow(( ptCar[1+3].y - Cmoy.y),2) );
			ptPol[5].r = sqrt( pow(( ptCar[2+3].x-Cmoy.x),2) + pow(( ptCar[2+3].y - Cmoy.y),2) );

			ptPol[0].teta = acos(  ( ptCar[0].x-Cmoy.x) / ptPol[0].r);
			ptPol[1].teta = acos( (ptCar[1].x-Cmoy.x) / ptPol[1].r);
			ptPol[2].teta = acos( (ptCar[2].x-Cmoy.x) / ptPol[2].r);
			ptPol[3].teta = acos( ( ptCar[0+3].x-Cmoy.x) / ptPol[3].r);
			ptPol[4].teta = acos( ( ptCar[1+3].x-Cmoy.x) / ptPol[4].r);
			ptPol[5].teta = acos( ( ptCar[2+3].x-Cmoy.x) / ptPol[5].r);
			float delta[3];
			for(int i = 0 ; i < 3 ; i++)
			{
				delta[i] = ptPol[i+3].teta - ptPol[i].teta;
				cout << endl << delta[i] << ":"<<i;
			}

			Droite ou gauche Si delta > 0 : gauche sinon droite si delta trop proche de 0 rien du tout
			Face donc sur l'axe d'openGL
			float sommeDelta = 0; 
			float moyDelta =0; int nbDelta=0;
			for(int i =0; i <3 ; i++)
			{
				sommeDelta += delta[i];  nbDelta++;
			}
			moyDelta =sommeDelta / nbDelta;
			cout << "d :" << moyDelta << endl;
		
			if(abs(moyDelta) > sensiRot)
			if(moyDelta < 0) 
			{
				r->moveRX(cranRotation);
			}else
				r->moveRX(-cranRotation);
				*/
}