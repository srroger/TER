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

/**
Cette classe permet de faire la liaison entre le traitement de l'image et les mouvements du cubes.
Elle fera aussi les pré-traitements dans cette optiques
*/
class Interpreteur
{
public:
	Viewer* v;
	RubixCube* r;
	Interpreteur();
	Interpreteur(Viewer* vi , RubixCube* ru);
	~Interpreteur(void);

	void translation();
};

#endif