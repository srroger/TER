#ifndef CENTRE_H
#define CENTRE_H

#include "vector"
#include "opencv2/highgui/highgui.hpp"
#include<cv.h>

using namespace cv; 
using namespace std;

class Centre
{
public:
	Centre();
	~Centre();

	CvPoint point;
	CvScalar couleur;
	int couleurFacile; // Prend 0 pour Rouge, 1 pour Vert, 2 pour Bleu
	#define ROUGE 0
	#define VERT 1
	#define BLEU 2
	#define RIEN 3

	int H;
	int W;
};
#endif