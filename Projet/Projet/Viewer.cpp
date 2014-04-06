#include "stdafx.h"
#include "Viewer.h"

Viewer::Viewer(double alpha, double beta, double radius, double min_radius, RubixCube* ru)
{
	v_alpha = alpha; v_beta = beta; v_radius = radius; v_min_radius = min_radius; r =ru;
	cranRotationViewer = 3;

	posTrans[0] = 0 ;
	posTrans[1] = 0 ;
	posTrans[2] = 0 ;
	set_position();
}
void Viewer::angle_range_check()
{
	if (v_alpha >= 360)
		v_alpha -= 360;
	else if (v_alpha<0)
		v_alpha += 360;

	if (v_beta >= 360)
		v_beta -= 360;
	else if (v_beta<0)
		v_beta += 360;
}
void Viewer::set_position()
{
	double degtorad = 3.1415926535897932384626433832795 / 180.0;
	angle_range_check();
	pos[0] = (v_radius * cos(degtorad * v_beta)) * cos(degtorad * v_alpha) + posTrans[0];
	pos[1] = v_radius * sin(degtorad * v_beta) + posTrans[1];
	pos[2] = (v_radius * cos(degtorad * v_beta)) * sin(degtorad * v_alpha) + posTrans[2];
}



//Les rotation totale avec OpenGL se font en degree et non en radian
void Viewer::Rotation(int sens,int val)
{
	switch(sens)
	{
	case GAUCHE : 
		v_alpha += val; set_position(); glutPostRedisplay(); break;
	case DROITE : 
		v_alpha -= val; set_position(); glutPostRedisplay(); break;
	case HAUT : 
		v_beta += val; set_position(); glutPostRedisplay(); break;
	case BAS : 
		v_beta -= val; set_position(); glutPostRedisplay(); break;
	}

}

void Viewer::special_keyboard(int key)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		r->moveZ(2); 
		//posTrans[2] += 2;
		break;
	case GLUT_KEY_RIGHT:
		r->moveZ(-2); 
		//posTrans[2] -= 2;
		break;
	case GLUT_KEY_DOWN:
		r->moveY(-2); 
		//posTrans[1] -= 2;
		break;
	case GLUT_KEY_UP: 
		r->moveY(2); 
		//posTrans[1] += 2;
		break;
	case GLUT_KEY_PAGE_UP:
		r->moveRZ(3); 
		//Rotation(HAUT,3);
		break;
	case GLUT_KEY_PAGE_DOWN:
		r->moveRY(3); 
		//Rotation(GAUCHE,3);
		break;
	}
	set_position(); glutPostRedisplay(); 
}

void Viewer::reset_view(double alpha, double beta, double radius, double min_radius)
{
	v_alpha = alpha; v_beta = beta; v_radius = radius; v_min_radius = min_radius; r->reinit();
	set_position(); glutPostRedisplay();
}
void Viewer::LookAtCentre()
{
	if (v_beta>90 && v_beta<270)
		gluLookAt(pos[0], pos[1], pos[2], posTrans[0],posTrans[1],posTrans[2], 0, -1, 0);
	else
		gluLookAt(pos[0], pos[1], pos[2], posTrans[0],posTrans[1],posTrans[2], 0, 1, 0);
}