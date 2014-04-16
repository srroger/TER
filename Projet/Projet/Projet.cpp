//#include "opencv2/core/core.hpp"
//#include "opencv2/flann/miniflann.hpp"
//#include "opencv2/imgproc/imgproc.hpp"
//#include "opencv2/photo/photo.hpp"
//#include "opencv2/video/video.hpp"
//#include "opencv2/features2d/features2d.hpp"
//#include "opencv2/objdetect/objdetect.hpp"
//#include "opencv2/calib3d/calib3d.hpp"
//#include "opencv2/ml/ml.hpp"
//#include "opencv2/contrib/contrib.hpp"
//#include "opencv2/imgproc/imgproc.hpp"
//#include "opencv2/highgui/highgui.hpp"
//#include <stdlib.h>

#include <stdio.h>
#include "stdafx.h"
#include "opencv2/highgui/highgui.hpp"
#include<cv.h>
#include <iostream>
#include <time.h>
#include<stack>


#include <cmath>
#include "cstdlib"
#include "ctime"
#include "vector"
#include "windows.h"
#include "glut.h"
#include "Cube.h"
#include "RubixCube.h"
#include "Viewer.h"
#include "Interpreteur.h"
#include "Centre.h"

using namespace std;
using namespace cv; 

/*-------------------------------------------------------------------------------------*/
/*-------------------------------OPENCV -----------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

 
IplImage *image,*hsv;

// Color tracked and our tolerance towards it
int hR = 10, sR = 245, vR = 0, tolerance = 15;
int hG = 64,sG = 115 ,vG = 0;
int hB = 116 ,sB = 177 ,vB = 0;

bool initialisation = true; int cptInit = 0; bool say = true; // Variable pour l'initialisation des couleurs au debut du programme
double debut, fin; 

int Xmax=0,Xmin=1000,Ymax=0,Ymin=1000;
/**
struct Centre{
	CvPoint point;
	CvScalar couleur;
	int H;
	int W;
};*/

IplImage* binarisation(IplImage* image);
vector<Centre> detection(IplImage* mask);
void DrawCentre(Centre* TabCentre);
void Inverse();
void MaxMin(int x, int y);
vector<Centre> Tracking(vector<Centre> TabCentre);
vector<CvPoint> direction(vector<Centre> tabCentre, vector<Centre> tabNewCentre);

//Objet rendu static pour la liaisons :
// Touche clavier
char keyb=NULL;
// Capture vidéo
static CvCapture *capture;
static	double d, f;
static	IplImage *imageBis;
static	vector<Centre> tabCentre;

static Interpreteur interprete;


bool preBin(IplImage* image) {
 
    CvScalar pixel; // element valeur d'un pixel
    IplImage *mask;
    IplConvKernel *kernel;
	
    // Create the mask &initialize it to white (no color detected)
    mask = cvCreateImage(cvGetSize(image), IPL_DEPTH_8U, 1); //image->depth = type de donnée de //l'image

	int x = 0;int pasX = hsv->width / 7 ;
	int y = 0;int pasY = hsv->height / 20;
	///cout<<hsv->width<<"  "<<hsv->height<<endl;
	bool res = false;
    // We create the mask
	while (x<hsv->width && !res)
    {
		y =0;
        while (y<hsv->height && !res )
        {
            pixel=cvGet2D(hsv, y, x);
            if ( ((pixel.val[0]>=hR-tolerance && pixel.val[1]>=sR-tolerance)&&(pixel.val[0]<=hR+tolerance && pixel.val[1]<=sR+tolerance))
				||((pixel.val[0]>=hB-tolerance && pixel.val[1]>=sB-tolerance)&&(pixel.val[0]<=hB+tolerance && pixel.val[1]<=sB+tolerance))
				||((pixel.val[0]>=hG-tolerance && pixel.val[1]>=sG-1.5*tolerance)&&(pixel.val[0]<=hG+tolerance && pixel.val[1]<=sG+1.5*tolerance)) )
    //        if ( ((pixel.val[0]>=hR-tolerance && pixel.val[1]>=toleranceSaturationR)&&(pixel.val[0]<=hR+tolerance ))
				//||((pixel.val[0]>=hB-tolerance && pixel.val[1]>=toleranceSaturationB)&&(pixel.val[0]<=hB+tolerance ))
				//||((pixel.val[0]>=hG-tolerance && pixel.val[1]>=toleranceSaturationG)&&(pixel.val[0]<=hG+tolerance )) )
            {  //pixel.val[0]=255; cvSet2D(mask, y, x,pixel);
				res =true;
            }
            else {
                //pixel.val[0]=0; cvSet2D(mask, y, x,pixel);
			}

			y= y + pasY;
        }

		x = x + pasX;
    }
	 
	return res;
}

/*---------------------------------------------
/*
 * Transform the image into a two colored image, one color for the color we want to track, another color for the others colors
 */
IplImage* binarisation(IplImage* image) {
 
    CvScalar pixel; // element valeur d'un pixel
    IplImage *mask;
    IplConvKernel *kernel;
	
    // Create the mask &initialize it to white (no color detected)
    mask = cvCreateImage(cvGetSize(image), IPL_DEPTH_8U, 1); //image->depth = type de donnée de //l'image

    // We create the mask
	 for (int x=0;x<hsv->width; x++)
    {
        for (int y=0; y<hsv->height; y++)
        {
            pixel=cvGet2D(hsv, y, x);
            if ( ((pixel.val[0]>=hR-tolerance && pixel.val[1]>=sR-tolerance)&&(pixel.val[0]<=hR+tolerance && pixel.val[1]<=sR+tolerance))
				||((pixel.val[0]>=hB-tolerance && pixel.val[1]>=sB-tolerance)&&(pixel.val[0]<=hB+tolerance && pixel.val[1]<=sB+tolerance))
				||((pixel.val[0]>=hG-tolerance && pixel.val[1]>=sG-1.5*tolerance)&&(pixel.val[0]<=hG+tolerance && pixel.val[1]<=sG+1.5*tolerance)) )
    //        if ( ((pixel.val[0]>=hR-tolerance && pixel.val[1]>=toleranceSaturationR)&&(pixel.val[0]<=hR+tolerance ))
				//||((pixel.val[0]>=hB-tolerance && pixel.val[1]>=toleranceSaturationB)&&(pixel.val[0]<=hB+tolerance ))
				//||((pixel.val[0]>=hG-tolerance && pixel.val[1]>=toleranceSaturationG)&&(pixel.val[0]<=hG+tolerance )) )
            {  pixel.val[0]=255; cvSet2D(mask, y, x,pixel);
            }
            else {
                pixel.val[0]=0; cvSet2D(mask, y, x,pixel);
			}
			
        }
    }
	 
	// Create kernels for the morphological operation --------------- noyau de base (3,3)
    kernel = cvCreateStructuringElementEx(10, 10, 4, 4, CV_SHAPE_ELLIPSE);
 
    // Morphological opening and closing to remove the holes and the little object
    cvDilate(mask, mask, kernel, 1);
    cvErode(mask, mask, kernel, 1);
	cvErode(mask, mask, kernel, 1);
	cvDilate(mask, mask, kernel, 1);
	 // We release the memory of kernels
    cvReleaseStructuringElement(&kernel);

    // Show the result of the mask image
    //cvShowImage("GeckoGeek Mask", mask);

	return mask;
}


vector<Centre> detection(IplImage* mask){

	IplImage *bin=cvCloneImage(mask);
	vector<Centre> TabCentre;
	Centre b;
	CvPoint P;
	CvScalar pixel;
	int nbPixel=0,Xi,Yi,label=-1;
	stack<CvPoint> pile;
	
	//ICI SOUCIS label ????!!!!!
	// plus 6 mais 3
	for(int  x=0; x<bin->width && label<3;++x)
		for(int y=0;y<bin->height && label <3;++y){
			if( ((uchar *)(bin->imageData + y*bin->widthStep))[x]  == 255  ){ // (uchar *)(mask->imageData + y*mask->widthStep))[x] 
					P.x=x;
					P.y=y;

					pile.push(P);
					nbPixel=0;
					Xi=0;
					Yi=0;
					label++;
					Xmax=0,Xmin=image->width,Ymax=0,Ymin=image->height;
				
				while(!pile.empty()){
					P = pile.top();
					pile.pop();
					pixel.val[0]=0;
					int yP=P.y;
					int xP=P.x;
					cvSet2D(bin,yP,xP,pixel);	//metttre a noir
					Xi+=xP;
					Yi+=yP;
					nbPixel++;
					MaxMin(xP,yP);

					// we watch the 8 neighbour
					if(xP+1<bin->width && cvGet2D(bin, yP, xP+1).val[0] == 255){
						P.x=xP+1;
						P.y=yP;
						cvSet2D(bin, yP, xP+1,pixel);
						pile.push(P);
					}
					if(xP-1>=0 && cvGet2D(bin, yP, xP-1).val[0] == 255){
						P.x=xP-1;
						P.y=yP;
						cvSet2D(bin, yP, xP-1,pixel);
						pile.push(P);
					}
					if(yP+1<bin->height && cvGet2D(bin, yP+1, xP).val[0] == 255){
						P.x=xP;
						P.y=yP+1;
						cvSet2D(bin, yP+1, xP,pixel);
						pile.push(P);
					}
					if(yP-1>=0 && cvGet2D(bin, yP-1, xP).val[0] == 255){
						P.x=xP;
						P.y=yP-1;
						cvSet2D(bin, yP-1, xP,pixel);
						pile.push(P);
					}


					if(yP-1>=0 && xP-1>=0 && cvGet2D(bin, yP-1, xP-1).val[0] == 255){
						P.x=xP-1;
						P.y=yP-1;
						cvSet2D(bin, yP-1, xP-1,pixel);
						pile.push(P);
					}
					if(yP+1<bin->height && xP+1<bin->width && cvGet2D(bin, yP+1, xP+1).val[0] == 255){
						P.x=xP+1;
						P.y=yP+1;
						cvSet2D(bin, yP+1, xP+1,pixel);
						pile.push(P);
					}
					if(yP+1<bin->height && xP-1>=0 && cvGet2D(bin, yP+1, xP-1).val[0] == 255){
						P.x=xP-1;
						P.y=yP+1;
						cvSet2D(bin, yP+1, xP-1,pixel);
						pile.push(P);
					}
					if(yP-1>=0 && xP+1<bin->width && cvGet2D(bin, yP-1, xP+1).val[0] == 255){
						P.x=xP+1;
						P.y=yP-1;
						cvSet2D(bin, yP-1, xP+1,pixel);
						pile.push(P);
					}
					

				}///finwhile
				
				
				if(pile.empty()){
					b.point=cvPoint((int)Xi/nbPixel,(int)Yi/nbPixel);
					b.couleur=cvGet2D(hsv, (int)Yi/nbPixel, (int)Xi/nbPixel);
					if ( ((pixel.val[0]>=hR-tolerance && pixel.val[1]>=sR-tolerance)&&(pixel.val[0]<=hR+tolerance && pixel.val[1]<=sR+tolerance)))
						b.couleurFacile = ROUGE;
					if((pixel.val[0]>=hB-tolerance && pixel.val[1]>=sB-tolerance)&&(pixel.val[0]<=hB+tolerance && pixel.val[1]<=sB+tolerance))
						b.couleurFacile = BLEU;
					if((pixel.val[0]>=hG-tolerance && pixel.val[1]>=sG-1.5*tolerance)&&(pixel.val[0]<=hG+tolerance && pixel.val[1]<=sG+1.5*tolerance))
						b.couleurFacile = VERT;

					b.H=Ymax-Ymin;
					b.W=Xmax-Xmin;

					TabCentre.push_back(b);
				}
			}//finif
		}//finfor
		 cvReleaseImage(&bin);
		 cvReleaseImage(&mask);

		return TabCentre;
}
 


void DrawCentre(vector<Centre> TabCentre){

	for(int i=0;i < TabCentre.size();i++)
	{
		/*if( ((TabCentre[i].couleur.val[0]>=hR-tolerance && TabCentre[i].couleur.val[1]>=sR-tolerance)&&(TabCentre[i].couleur.val[0]<=hR+tolerance && TabCentre[i].couleur.val[1]<=sR+tolerance))
			||((TabCentre[i].couleur.val[0]>=hB-tolerance && TabCentre[i].couleur.val[1]>=sB-tolerance)&&(TabCentre[i].couleur.val[0]<=hB+tolerance && TabCentre[i].couleur.val[1]<=sB+tolerance))
			||((TabCentre[i].couleur.val[0]>=hG-tolerance && TabCentre[i].couleur.val[1]>=sG-1.5*tolerance)&&(TabCentre[i].couleur.val[0]<=hG+tolerance && TabCentre[i].couleur.val[1]<=sG+1.5*tolerance)) )
		{
		
			cvDrawCircle(image, TabCentre[i].point, 5, CV_RGB(0, 0, 0), -1);
		}
		*/

		if( ((TabCentre[i].couleur.val[0]>=hR-tolerance && TabCentre[i].couleur.val[1]>=sR-tolerance)&&(TabCentre[i].couleur.val[0]<=hR+tolerance && TabCentre[i].couleur.val[1]<=sR+tolerance)))
			cvDrawCircle(image, TabCentre[i].point, 20, CV_RGB(255, 0, 0), 2);
			
		if(((TabCentre[i].couleur.val[0]>=hB-tolerance && TabCentre[i].couleur.val[1]>=sB-tolerance)&&(TabCentre[i].couleur.val[0]<=hB+tolerance && TabCentre[i].couleur.val[1]<=sB+tolerance)))
			cvDrawCircle(image, TabCentre[i].point, 20, CV_RGB(0, 0, 255), 2);

		if(((TabCentre[i].couleur.val[0]>=hG-tolerance && TabCentre[i].couleur.val[1]>=sG-1.5*tolerance)&&(TabCentre[i].couleur.val[0]<=hG+tolerance && TabCentre[i].couleur.val[1]<=sG+1.5*tolerance)) )
			cvDrawCircle(image, TabCentre[i].point, 20, CV_RGB(0, 255, 0), 2);


		if( !((TabCentre[i].couleur.val[0]>=hR-tolerance && TabCentre[i].couleur.val[1]>=sR-tolerance)&&(TabCentre[i].couleur.val[0]<=hR+tolerance && TabCentre[i].couleur.val[1]<=sR+tolerance))
			//cvDrawCircle(image, TabCentre[i].point, 20, CV_RGB(255, 0, 0), 2);
			
		 && !((TabCentre[i].couleur.val[0]>=hB-tolerance && TabCentre[i].couleur.val[1]>=sB-tolerance)&&(TabCentre[i].couleur.val[0]<=hB+tolerance && TabCentre[i].couleur.val[1]<=sB+tolerance))
			//cvDrawCircle(image, TabCentre[i].point, 20, CV_RGB(0, 0, 255), 2);

		&& !((TabCentre[i].couleur.val[0]>=hG-tolerance && TabCentre[i].couleur.val[1]>=sG-1.5*tolerance)&&(TabCentre[i].couleur.val[0]<=hG+tolerance && TabCentre[i].couleur.val[1]<=sG+1.5*tolerance)) )
			cvDrawCircle(image, TabCentre[i].point, 20, CV_RGB(120, 120, 120), 2);


	}
}


//permet de recuperer les extremum de l'object pour avoir sa hauteur et sa largueur par la suite
void MaxMin(int x, int y){
	if(x>Xmax)
		Xmax=x;
	if(x<Xmin)
		Xmin=x;
	if(y>Ymax)
		Ymax=y;
	if(y<Ymin)
		Ymin=y;
}




vector<Centre> Tracking(vector<Centre> TabCentre){

	int Xi,Yi,nbPixel;
	Centre b;
	vector<Centre> TabNewB;

	int Xdebut,Xfin,Ydebut,Yfin;

	for(int i=0;i<TabCentre.size();i++){
		Xi=0;
		Yi=0;
		nbPixel=0;
		Xmax=0,Xmin=image->width,Ymax=0,Ymin=image->height;
		
		// ou -+2*w  et -+2*h

		if(TabCentre[i].point.x - TabCentre[i].W <0)
			Xdebut= 0;
		else Xdebut=TabCentre[i].point.x - TabCentre[i].W ;
		
		if(TabCentre[i].point.x + TabCentre[i].W >=image->width)
			 Xfin= image->width;
		else  Xfin=TabCentre[i].point.x + TabCentre[i].W;

		if(TabCentre[i].point.y - TabCentre[i].H <0)
			 Ydebut=0;
		else Ydebut=TabCentre[i].point.y - TabCentre[i].H;

		if(TabCentre[i].point.y + TabCentre[i].H >=image->height)
			 Yfin=image->height;
		else Yfin=TabCentre[i].point.y + TabCentre[i].H;


		for(int x= Xdebut; x<Xfin ;++x){
			for(int y= Ydebut ; y<Yfin ;++y){
				if( TabCentre[i].couleur.val[0] <=  cvGet2D(hsv, y, x).val[0] + tolerance  && TabCentre[i].couleur.val[0] >=  cvGet2D(hsv, y, x).val[0] - tolerance 
					&& TabCentre[i].couleur.val[1] <=  cvGet2D(hsv, y, x).val[1] + tolerance  && TabCentre[i].couleur.val[1] >=  cvGet2D(hsv, y, x).val[1] - tolerance ){ // pb de la couleur
					

					Xi+=x;
					Yi+=y;
					nbPixel++;
					MaxMin(x,y);
				}
			}
		}
		
		if(nbPixel!=0){
			b.H=Xmax-Xmin;
			b.W=Ymax-Ymin;
			b.point.x=Xi/nbPixel;
			b.point.y=Yi/nbPixel;
			b.couleur=cvGet2D(hsv, b.point.y, b.point.x);
			if ( ((b.couleur.val[0]>=hR-tolerance && b.couleur.val[1]>=sR-tolerance)&&(b.couleur.val[0]<=hR+tolerance && b.couleur.val[1]<=sR+tolerance)))
				b.couleurFacile = ROUGE;
			if((b.couleur.val[0]>=hB-tolerance && b.couleur.val[1]>=sB-tolerance)&&(b.couleur.val[0]<=hB+tolerance && b.couleur.val[1]<=sB+tolerance))
				b.couleurFacile = BLEU;
			if((b.couleur.val[0]>=hG-tolerance && b.couleur.val[1]>=sG-1.5*tolerance)&&(b.couleur.val[0]<=hG+tolerance && b.couleur.val[1]<=sG+1.5*tolerance))
				b.couleurFacile = VERT;
			TabNewB.push_back(b);
		}
	}

	return TabNewB;
}


/*
 * Get the color of the pixel where the mouse has clicked
 */
void getObjectColor(int event, int x, int y, int flags, void *param = NULL) {
 
    // Vars
    CvScalar pixel;
    IplImage *hsv;
 
    if(event == CV_EVENT_LBUTTONUP) {
 
        // Get the hsv image
        hsv = cvCloneImage(image);
        cvCvtColor(image, hsv, CV_BGR2HSV);
 
        // Get the selected pixel
        pixel = cvGet2D(hsv, y, x); // hsv
		//cout <<"r "<< (int)pixel.val[0] <<" g "<<(int)pixel.val[1]<<" b "<<(int)pixel.val[2]<< endl;
		//cout <<"h "<< (int)pixel.val[0] <<" s "<<(int)pixel.val[1]<<" v "<<(int)pixel.val[2]<< endl;
        // Release the memory of the hsv image
            cvReleaseImage(&hsv);
 
		if(initialisation)
		{
			switch (cptInit)
			{
			case 0 : hR =  (int)pixel.val[0]; sR = (int)pixel.val[1];say = true; cptInit++; break; 
			case 1 : hG =  (int)pixel.val[0]; sG = (int)pixel.val[1];say = true; cptInit++; break;
			case 2 : hB =  (int)pixel.val[0]; sB = (int)pixel.val[1];say = true; cptInit++; break;
			}
		}
    }
 
}
 
/*void Inverse(){
	IplImage* ImgInv= cvCloneImage(image);//cvCreateImage(cvGetSize(image), IPL_DEPTH_8U, 1);
	CvScalar pixel1;
	CvScalar pixel2;
	int w=image->width;
	for (int y=0; y<image->height; ++y)
	{
		for (int x=0;x<image->width; ++x){
			pixel1= cvGet2D(image,y,x);
			for (int k = 0; k < image->nChannels; ++k){
				pixel2.val[k] = pixel1.val[k];
			}
			cvSet2D(ImgInv,y,w-1 - x,pixel2);//cout<<pixel<<endl;
		}
	 }
	  //cvShowImage("ImgInv", ImgInv);
	 image=cvCloneImage(ImgInv);
	 cvReleaseImage(&ImgInv);
}*/







/*--------------------------------------------------------------------------------------*/
/*-------------------------------OPENGL PARTIE ----------------------------------------*/
/*--------------------------------------------------------------------------------------*/




int resx = 1000, resy = 800;


void reshape(int w, int h);
void init();
void key(unsigned char key, int x, int y);
void specialkey(int key, int x, int y);
void display();
void change_pos(int dir, int & a0, int & a1, int & a2, int & b0, int & b1, int & b2, int & c0, int & c1, int & c2);
void idle();
void processNormalKeys(unsigned char key, int x, int y);

//static Viewer v(45, 45, 20, 10);


void change_pos(int dir, int & a0, int & a1, int & a2, int & b0, int & b1, int & b2, int & c0, int & c1, int & c2)
{//change_pos(dir,cubepos[plane.back()][0][0],cubepos[plane.back()][0][1],cubepos[plane.back()][0][2],cubepos[plane.back()][1][0],cubepos[plane.back()][1][1],cubepos[plane.back()][1][2],cubepos[plane.back()][2][0],cubepos[plane.back()][2][1],cubepos[plane.back()][2][2]);

	int temp;
	if (dir)
	{
		temp = a0; a0 = a2; a2 = c2; c2 = c0; c0 = temp;
		temp = b0; b0 = a1; a1 = b2; b2 = c1; c1 = temp;
	}
	else
	{
		temp = a0; a0 = c0; c0 = c2; c2 = a2; a2 = temp;
		temp = b0; b0 = c1; c1 = b2; b2 = a1; a1 = temp;
	}
}


RubixCube r(2);
//static Viewer v(0, 30, 40, 10,&r);
static Viewer v(0, 0, 40, 10,&r);
//int main(int argc, char * argv[])
//{
//	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
//	glutInitWindowSize(resx, resy);
//	//glutInitWindowPosition(900,0);
//	glutCreateWindow("Rubik's Cube");
//	glutDisplayFunc(display);
//	glutReshapeFunc(reshape);
//	glutKeyboardFunc(processNormalKeys);
//	glutSpecialFunc(specialkey);
//	init();
//	glutMainLoop();
//}


void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, (GLfloat)w / (GLfloat)h, 1, 200);
	glMatrixMode(GL_MODELVIEW);
}


void key(unsigned char key,int x ,int y)
{
	switch(key)
	{
	case 'R':
	case 'r':
		v.reset_view(45,45,20,10);
		break;
	case 27:
		//if(conformationbox(L"Exit?",L"Do You Want To Exit?"))
			exit(0);
		break;
	case 'n':
	case 'N':
		//if(conformationbox(L"New Game?",L"Do You Want To Start A New Game?"))
		//{
			r.randomize();
			glutPostRedisplay();
		//}
		break;
	case '.': case '>':
	case ',': case '<':
	case 'x': case 'X':	case 'c': case 'C':
	case 'y': case 'Y':	case 'u': case 'U':
	case 'z': case 'Z':	case 'e': case 'E':
	case '1': case '2': case '3':
		r.keyboard(key);
		glutPostRedisplay();
		break;
	case 's':
	case 'S':
		if(!r.solving)
			//if(!conformationbox(L"Solve Cube?",L"Do You Want The Computer To Solve The Cube?"))
				break;
		r.keyboard(key);
		glutPostRedisplay();
		break;
	case 'i':
			r = RubixCube((double)2);
			cout<<"Fait espace" <<endl;
			break;
	}
}

void specialkey(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT: case GLUT_KEY_RIGHT:
	case GLUT_KEY_DOWN:	case GLUT_KEY_UP:
	case GLUT_KEY_PAGE_UP:case GLUT_KEY_PAGE_DOWN:
	v.special_keyboard(key); break;
	}
}
void processNormalKeys(unsigned char key, int x, int y) {

	if (key == 27)
		exit(0);
}
void init()
{
	
	glClearColor(0.2, 0.2, 0.2, 1.0);
	glEnable(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	v.LookAtCentre();

	glTranslatef(0, 0, 0);
	r.display();
	

	glFlush();
	glutSwapBuffers();
	
}



void on_opengl(int argc, char * argv[]) { 

		
	resx = cvGetCaptureProperty(capture,CV_CAP_PROP_FRAME_WIDTH);
	resy = cvGetCaptureProperty(capture,CV_CAP_PROP_FRAME_HEIGHT);

		//OPENGL INITAILISATION;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(resx , resy); glutInitWindowPosition(650,500);
	glutCreateWindow("Rubik's Cube"); 
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(key);
	glutSpecialFunc(specialkey);
	glutIdleFunc(idle);
	init();
	
	//glutMainLoop();

}



int redo = 0; int numberBeforeRedo = 5; // Ces variables permette de ne pas refaire de binarisation enchainer
void idle()
{
	r.rotation_idle_func();
			//cout << "Tour "<<endl;
			// Boucle tant que l'utilisateur n'appuie pas sur la touche q (ou Q)
			//while(keyb != 'q' && keyb != 'Q') {
			
	// On récupère une image
	image = cvQueryFrame(capture); 
	hsv = cvCloneImage(image);
	cvCvtColor(image, hsv, CV_BGR2HSV);

	if(initialisation)
	{
		if(say)
		switch (cptInit)
		{
		case 0 : cout << "Cliquez sur le Rouge" <<endl  ;
			say = false; break; 
		case 1 : cout << "Cliquez sur le Vert" <<endl ;
			say = false; break; ; 
		case 2 : cout << "Cliquez sur le Bleu" <<endl  ;
			say = false;  break; ; 
		case 3 : initialisation = false ;
		}
		cvFlip(image,image,-1); 
		cvShowImage("GeckoGeek Color Tracking", image);
		

		cvReleaseImage(&hsv);
	}
	else
	{
			
		redo++;
		cout<<redo<<"                                                    "<<tabCentre.size()<<endl;
					/// avant c'etait 6 mais comme on fait tout avec 3 doigts

		// voir lequel des deux est le mieux!!!
				//if( (tabCentre.size()==0 && redo >= numberBeforeRedo)|| (((double)(fin-debut) / (double) CLOCKS_PER_SEC)>0.3)  || tabCentre.size()>3 ){ //!!!!!
		if( (tabCentre.size()==0 && preBin(image) )|| (((double)(fin-debut) / (double) CLOCKS_PER_SEC)>0.3)  || tabCentre.size()>3 ){
			cout << "Fait binarisation \n";
					//debut = clock();
			imageBis=binarisation(image);
					//fin = clock(); 
					//cout<<" 2   binarisation : "<<endl;//<<((double)(fin-debut) / (double) CLOCKS_PER_SEC)<<endl; 
					//debut = clock();
			tabCentre=detection(imageBis);
			//DrawCentre(tabCentre);
				
						//fin = clock(); 
						//cout<<" 2   detection : "<<((double)(fin-debut) / (double) CLOCKS_PER_SEC)<<endl;
			redo=0;
		}

					// If there is no image, we exit the loop
					//if(!image)
					//	continue;
		
		vector<Centre> tabnewCentre;
		debut = clock();
		tabnewCentre=Tracking(tabCentre);
		fin = clock(); 
		cout<<"				   tracking : "<<((double)(fin-debut) / (double) CLOCKS_PER_SEC)<<endl; 

		DrawCentre(tabnewCentre);
					
					//IplImage* flipped;
		cvFlip(image,image,-1); // Effet miroir sur l'image de la camera a decommenter quand le logiciel sera fini pour avoir un affichage ergonomique
		cvShowImage("GeckoGeek Color Tracking", image);
		cvReleaseImage(&hsv);

		// INTERPRETATION
				//r.display_rotation();
				//r.moveRX(0.5);

		if(tabCentre.size()==tabnewCentre.size() && redo>=2){
			//DrawCentre(tabnewCentre);
			cout<<"ok c'est bon!!!!"<<endl;
			interprete.launch(tabCentre,tabnewCentre);
					//cout << "Apres launch" << endl;
		}
		//FIN INTERPRETATION

		//copie du nouveau tableau dans tabCentre
		tabCentre.clear();
		for(int i=0;i<tabnewCentre.size();i++){
			tabCentre.push_back(tabnewCentre[i]);
		}

		
			
		// On attend 10ms
		keyb = cvWaitKey(5);
	}
		


//}
		
	glutPostRedisplay();
}


/*--------------------------------------------------------------------------------------*/
/*-------------------------------LIAISON -------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/
//TabCentre
//TabNumCentre
//Utilisation de la classe interpreteur


/*--------------------------------------------------------------------------------------*/
/*-------------------------------MAIN -------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/


int main( int argc, char * argv[])
{


	interprete = Interpreteur(&v,&r, hR, sR, hG, sG, hB, sB);
	//debut = clock();
	
    // Ouvrir le flux vidéo
    capture = cvCreateCameraCapture(CV_CAP_ANY);
	//capture = cvCreateCameraCapture(2);
	
    // Vérifier si l'ouverture du flux est ok
    if (!capture) {
 
       printf("Ouverture du flux vidéo impossible !\n");
       return 1;
 
    }

	
	// Create the windows
    cvNamedWindow("GeckoGeek Color Tracking", CV_WINDOW_AUTOSIZE);
    //cvNamedWindow("GeckoGeek Mask", CV_WINDOW_AUTOSIZE);
    cvMoveWindow("GeckoGeek Color Tracking", 0, 300);
    //cvMoveWindow("GeckoGeek Mask", 650, 50);
	// Mouse event to select the tracked color on the original image
    cvSetMouseCallback("GeckoGeek Color Tracking", getObjectColor);
 

	// initialisation de la capture
		image = cvQueryFrame(capture);
		hsv = cvCloneImage(image);
		cvCvtColor(image, hsv, CV_BGR2HSV);
		
		debut = clock();
		imageBis=binarisation(image);
		fin = clock(); 
		//cout<<"  binarisation : "<<((double)(fin-debut) / (double) CLOCKS_PER_SEC)<<endl; 
		//debut = clock();
		tabCentre = vector<Centre>(detection(imageBis));
		DrawCentre(tabCentre);
		fin = clock(); 
		//cout<<"  detection : "<<((double)(fin-debut) / (double) CLOCKS_PER_SEC)<<endl;
		
  
		
		//Boucle (dans idle maintenant)
		on_opengl(argc,argv);
		//FIN OPENGL INITIALISATION
		glutMainLoop();

		// Boucle tant que l'utilisateur n'appuie pas sur la touche q (ou Q)
	


    cvReleaseCapture(&capture);
	cvDestroyWindow("GeckoGeek Color Tracking");
    //cvDestroyWindow("GeckoGeek Mask");


}

