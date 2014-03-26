#include "stdafx.h"
#include "Centre.h"

using namespace cv; 
using namespace std;

Centre::Centre(){
	point.x = 0;
	point.y = 0;
	couleur.val[0]=0;
	couleurFacile = -1;
	H = 0;
	W = 0;
}

Centre::~Centre()
{

}