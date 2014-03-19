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
	int H;
	int W;
};
#endif