#ifndef RUBIXCUBE_H
#define RUBIXCUBE_H
#include <cmath>
#include "cstdlib"
#include "ctime"
#include "vector"
#include "windows.h"
#include "glut.h"
#include "Cube.h"
#include <iostream>
#include <string>
#include <fstream>
 
using namespace std;
class RubixCube
{
	GLfloat x,y,z; // Ces valeur permettent de connaitre sa position par au centre de l'image.
	GLfloat rx,ry,rz; // L'angle de rotation.
	
	float AxisX[3];
	float AxisY[3];
	float AxisZ[3];

		Cube c[27];
	double len,theta;
	int dir, cubepos[3][3][3];
	bool rotating;
	std::vector<int> axis,plane,
		keypressed/*0=axis change, 1=plane change, 2=clockwise rot, 3=anticlockwise rot*/;


public:
	string ensembleCommandes;
	bool solving;
	RubixCube(double length);
	void set_centre();
	void display();
	void rotate_cube_plane();
	void display_rotation();
	void rotate_rubix();
	void rotation_idle_func();
	void keyboard(unsigned char key);
	friend void change_pos(int dir, int & a0, int & a1, int & a2, int & b0, int & b1, int & b2, int & c0, int & c1, int & c2);
	void randomize();
	void solve();
	bool check_solved();


	void moveZ(GLfloat val);
	void moveY(GLfloat val);
	void moveX(GLfloat val); //Profondeur
	void moveRX();
	void moveRY();
	void moveRZ();
	void moveRUF();
	void melange();
	void reinit();

	void recalcAxis();

private:


	//Ces fonctions simule les commande de mouvements mais sans les animations. (meme sens est un boolean qui permet de savoir si on veut faire le mouvement dans le même sens que si on le faisait avec les doigts ou dans le sens inverse)
	void moveR(bool memesens);
	void moveV(bool memesens);
	void moveB(bool memesens);
	void moveRV(bool memesens);

};

#endif
