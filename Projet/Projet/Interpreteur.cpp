#include "stdafx.h"
#include "Interpreteur.h"


Interpreteur::Interpreteur()
{
}
Interpreteur::Interpreteur(Viewer* vi , RubixCube* ru)
{
	this->v = vi;
	this->r = ru;
}


Interpreteur::~Interpreteur(void)
{
}


//Le point rouge est tout seul, et il se déplace
void Interpreteur::translation(void)
{


}

