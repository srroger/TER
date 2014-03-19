#include "stdafx.h"
#include "Interpreteur.h"

Interpreteur::Interpreteur()
{
	mode = IDLE;
}
Interpreteur::Interpreteur(Viewer* vi , RubixCube* ru)
{
	mode = IDLE;
	this->v = vi;
	this->r = ru;
}


Interpreteur::~Interpreteur(void)
{
}


struct PointGL{
	float x,y,z;
};

//Le point rouge est tout seul, et il se déplace
void Interpreteur::translation(void)
{
	//La size etant de 1 (normalement)

	//Changement de repère car dans opengl et openCV on a pas le meme repère x,y,z
	PointGL vect; 
	vect.x = 0;
	vect.y = -dir[0].y;
	vect.z = -dir[0].x;
	
	float seuille = 1;
	if(abs(vect.x) > seuille) r->moveX(vect.x);
	if(abs(vect.y) > seuille) r->moveY(vect.y);
	if(abs(vect.z) > seuille) r->moveZ(vect.z);

}

void Interpreteur::launch(vector<Centre> tabCentre, vector<Centre> tabNewCentre){

	if(tabNewCentre.size() != 0 )
	{
			
		dir = direction(tabCentre,tabNewCentre);
		mode = tabNewCentre.size();
		switch (mode)
		{
		case 0: {break; }//TODO
		case 1 :{ translation(); break;}
		case 3 : {break; }//TODO
			default :{ break;}
		}

	}
	

	
	//Repere le mode

}


vector<CvPoint> Interpreteur::direction(vector<Centre> tabCentre, vector<Centre> tabNewCentre){
	vector<CvPoint> DirDeplacement;
	for(int i=0;i<tabCentre.size();i++){
		CvPoint pt;
		pt.x= tabCentre.at(i).point.x - tabNewCentre.at(i).point.x;
		pt.y= tabCentre.at(i).point.y - tabNewCentre.at(i).point.y;
		DirDeplacement.push_back(pt);
	}

	return DirDeplacement;
}
