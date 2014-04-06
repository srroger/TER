#ifndef RUBIXCUBE_H
#define RUBIXCUBE_H
#include <cmath>
#include <iostream>
#include "cstdlib"
#include "ctime"
#include "vector"
#include "windows.h"
#include "glut.h"
#include "Cube.h"
using namespace std;




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

class RubixCube
{
	GLfloat x,y,z; // Ces valeur permettent de connaitre sa position par au centre de l'image.
	GLfloat rx,ry,rz; // L'angle de rotation.
	
	float AxisX[3];
	float AxisY[3];
	float AxisZ[3];


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
	void rotation_Test();
	void keyboard(unsigned char key);
	friend void change_pos(int dir, int & a0, int & a1, int & a2, int & b0, int & b1, int & b2, int & c0, int & c1, int & c2);
	

	void moveZ(GLfloat val);
	void moveY(GLfloat val);
	void moveX(GLfloat val); //Profondeur
	void moveRX(GLfloat val);
	void moveRY(GLfloat val);
	void moveRZ(GLfloat val);
	void reinit();

	void rotation_face(int face, int direction); 
	// Face est une variable qui va de 0 à 5 et qui désigne la face ducube a modifié. (donc dans cubepos, il s'agira d'un i bloqué ex i=0, ou un j, ou un k)
	// int direction determine le sens de la rotation, afin de mettre a jour cubepos

	void recalcAxis();
};

#endif
