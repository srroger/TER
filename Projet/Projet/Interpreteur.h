#ifndef INTERPRETEUR_H
#define INTERPRETEUR_H
#include <cmath>
#include "cstdlib"
#include "ctime"
#include "vector"
#include "windows.h"
#include "glut.h"
#include "RubixCube.h"
#include "Viewer.h"
#include "Centre.h"

#include "vector"
#include "opencv2/highgui/highgui.hpp"
#include<cv.h>

using namespace std;
using namespace cv;
/**
Cette classe permet de faire la liaison entre le traitement de l'image et les mouvements du cubes.
Elle fera aussi les pré-traitements dans cette optiques
*/

#define TRANSLATION_MODE 1
#define ROTATION_MODE 3
#define IDLE 0

class Interpreteur
{
public:
	Viewer* v;
	RubixCube* r;
	vector<CvPoint> dir;

	//dit si il est en mode translation / rotation / idle
	int mode;
	
	Interpreteur();
	Interpreteur(Viewer* vi , RubixCube* ru);
	~Interpreteur(void);

	void launch(vector<Centre> tabCentre, vector<Centre> tabNewCentre);

private:
	vector<CvPoint> direction(vector<Centre> tabCentre, vector<Centre> tabNewCentre);
	void translation();
};

#endif