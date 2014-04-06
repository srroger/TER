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

struct PointGL{
	float x,y,z;
	int couleurFacile;
};
struct PointPolaire{
	float r,teta;
	int couleurFacile;
};

class Interpreteur
{
public:
	Viewer* v;
	RubixCube* r;
	vector<CvPoint> dir;
	int hR, sR, tolerance;
	int hG ,sG;
	int hB,sB;
	bool faireRotation;
	int cptRotation; int maxRotation; //=90°

	PointGL C, C2;
	PointGL ptCar[6];// R,V,B,R2,V2,B2;
	PointPolaire ptPol[6]; //Rp,Vp,Bp,R2p,V2p,B2p;
	//bool testeur[6];int RefreshEvery; int cptRefreshFrame; //La distance en frame pour le calcul de la rotation
	int toleranceIdle; int RefreshEvery; int cptRefreshFrame; //Variable utile pour la reinitialisation, si il y a eu aucun mouvement avec 3 doigts pendant RefreEvery frame, on réinitilise le cube


	float sensiRot; int cranRotation;

	float seuilCote; // Si les delta sont inferieur a ce seuil, cela signifie qu'on fait une rotation sur le cote

	//dit si il est en mode translation / rotation / idle
	int mode;
	
	Interpreteur();
	Interpreteur(Viewer* vi , RubixCube* ru, int hr, int sr, int hg, int sg, int hb, int sb);
	~Interpreteur(void);

	void launch(vector<Centre> tabCentre, vector<Centre> tabNewCentre);

private:
	vector<CvPoint> direction(vector<Centre> tabCentre, vector<Centre> tabNewCentre);
	void translation(int indiceRouge);
	bool reinitialisation(vector<Centre> tabCentre, vector<Centre> tabNewCentre); //Reinitialise le cube si trois marqueur n'ont pas bougé pendant un laps de frame RefreshEvery
	void rotation(vector<Centre> tabCentre, vector<Centre> tabNewCentre);
	bool rotationFace(vector<Centre> tabCentre, vector<Centre> tabNewCentre);
	bool rotationCote(vector<Centre> tabCentre, vector<Centre> tabNewCentre);
	bool rotationBas(vector<Centre> tabCentre, vector<Centre> tabNewCentre);
};

#endif