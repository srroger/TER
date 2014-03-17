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


using namespace std;
using namespace cv; 


/*--------------------------------------------------------------------------------------*/
/*-------------------------------OPENCV -----------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

 
IplImage *image,*hsv;

// Color tracked and our tolerance towards it
int hR = 10, sR = 245, vR = 0, tolerance = 20;
int hG = 60,sG = 120 ,vG = 0 ;
int hB = 110 ,sB = 150 ,vB = 0;

double debut, fin; 

int Xmax=0,Xmin=1000,Ymax=0,Ymin=1000;

struct Centre{
	CvPoint point;
	CvScalar couleur;
	int H;
	int W;
};

IplImage* binarisation(IplImage* image);
vector<Centre> detection(IplImage* mask);
void DrawCentre(Centre* TabCentre);
void Inverse();
void MaxMin(int x, int y);
vector<Centre> Tracking(vector<Centre> TabCentre);


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
    cvShowImage("GeckoGeek Mask", mask);

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
	for(int  x=0; x<bin->width && label<6;++x)
		for(int y=0;y<bin->height && label <6;++y){
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

	for(int i=0;i<TabCentre.size();i++)
	{
		if( ((TabCentre[i].couleur.val[0]>=hR-tolerance && TabCentre[i].couleur.val[1]>=sR-tolerance)&&(TabCentre[i].couleur.val[0]<=hR+tolerance && TabCentre[i].couleur.val[1]<=sR+tolerance))
				||((TabCentre[i].couleur.val[0]>=hB-tolerance && TabCentre[i].couleur.val[1]>=sB-tolerance)&&(TabCentre[i].couleur.val[0]<=hB+tolerance && TabCentre[i].couleur.val[1]<=sB+tolerance))
				||((TabCentre[i].couleur.val[0]>=hG-tolerance && TabCentre[i].couleur.val[1]>=sG-1.5*tolerance)&&(TabCentre[i].couleur.val[0]<=hG+tolerance && TabCentre[i].couleur.val[1]<=sG+1.5*tolerance)) )
		{
			cvDrawCircle(image, TabCentre[i].point, 5, CV_RGB(0, 0, 0), -1);
		}
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
		cout <<"h "<< (int)pixel.val[0] <<" s "<<(int)pixel.val[1]<<" v "<<(int)pixel.val[2]<< endl;
        // Release the memory of the hsv image
            cvReleaseImage(&hsv);
 
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

Viewer v(45, 45, 20, 10);

void change_pos(int dir, int & a0, int & a1, int & a2, int & b0, int & b1, int & b2, int & c0, int & c1, int & c2)
{
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
		//OPENGL INITAILISATION
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(resx, resy);
	glutCreateWindow("Rubik's Cube");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(specialkey);
	glutIdleFunc(idle);
	init();
	
	//glutMainLoop();

}


	// Touche clavier
    char keyb=NULL;
    // Capture vidéo
    static CvCapture *capture;
	static	double d, f;
	static	IplImage *imageBis;
	static	vector<Centre> tabCentre;
void idle()
{
	r.rotation_idle_func();


		// Boucle tant que l'utilisateur n'appuie pas sur la touche q (ou Q)
	//while(keyb != 'q' && keyb != 'Q') {
			d=clock();
		// On récupère une image
		image = cvQueryFrame(capture);
		hsv = cvCloneImage(image);
		cvCvtColor(image, hsv, CV_BGR2HSV);

		cout<<"                                                    "<<tabCentre.size()<<endl;
		if(tabCentre.size()<6 && tabCentre.size()!=1){ //!!!!!
			debut = clock();
			imageBis=binarisation(image);
			fin = clock(); 
			cout<<" 2   binarisation : "<<((double)(fin-debut) / (double) CLOCKS_PER_SEC)<<endl; 
			debut = clock();
			tabCentre=detection(imageBis);
			DrawCentre(tabCentre);
			fin = clock(); 
			cout<<" 2   detection : "<<((double)(fin-debut) / (double) CLOCKS_PER_SEC)<<endl;

		}

		// If there is no image, we exit the loop
		//if(!image)
		//	continue;
		
		vector<Centre> tabnewCentre;
		debut = clock();
		tabnewCentre=Tracking(tabCentre);
		fin = clock(); 
			cout<<"				   tracking : "<<((double)(fin-debut) / (double) CLOCKS_PER_SEC)<<endl; 
		debut = clock();
		DrawCentre(tabnewCentre);
		fin = clock(); 
			cout<<"				   draw : "<<((double)(fin-debut) / (double) CLOCKS_PER_SEC)<<endl; 
		cvShowImage("GeckoGeek Color Tracking", image);
		

		cvReleaseImage(&hsv);


		//copie du nouveau tableau dans tabCentre
		tabCentre.clear();
		for(int i=0;i<tabnewCentre.size();i++){
			tabCentre.push_back(tabnewCentre[i]);
		}

		
		// On attend 10ms
		keyb = cvWaitKey(5);
		f=clock();
		cout<<"  tt : "<<((double)(f-d) / (double) CLOCKS_PER_SEC)<<endl; 



	//}

	glutPostRedisplay();
}

/*--------------------------------------------------------------------------------------*/
/*-------------------------------MAIN -------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/


int main( int argc, char * argv[])
{



	//debut = clock();
	
    // Ouvrir le flux vidéo
    capture = cvCreateCameraCapture(2);
	
    // Vérifier si l'ouverture du flux est ok
    if (!capture) {
 
       printf("Ouverture du flux vidéo impossible !\n");
       return 1;
 
    }

	
	// Create the windows
    cvNamedWindow("GeckoGeek Color Tracking", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("GeckoGeek Mask", CV_WINDOW_AUTOSIZE);
    cvMoveWindow("GeckoGeek Color Tracking", 0, 100);
    cvMoveWindow("GeckoGeek Mask", 650, 100);
	// Mouse event to select the tracked color on the original image
    cvSetMouseCallback("GeckoGeek Color Tracking", getObjectColor);
 

	// initialisation de la capture
		image = cvQueryFrame(capture);
		hsv = cvCloneImage(image);
		cvCvtColor(image, hsv, CV_BGR2HSV);
		
		debut = clock();
		imageBis=binarisation(image);
		fin = clock(); 
		cout<<"  binarisation : "<<((double)(fin-debut) / (double) CLOCKS_PER_SEC)<<endl; 
		//debut = clock();
		tabCentre = vector<Centre>(detection(imageBis));
		DrawCentre(tabCentre);
		fin = clock(); 
		cout<<"  detection : "<<((double)(fin-debut) / (double) CLOCKS_PER_SEC)<<endl;
		
  
		
		//Boucle (dans idle maintenant)
		on_opengl(argc,argv);
		//FIN OPENGL INITIALISATION
		glutMainLoop();

		// Boucle tant que l'utilisateur n'appuie pas sur la touche q (ou Q)
	


    cvReleaseCapture(&capture);
	cvDestroyWindow("GeckoGeek Color Tracking");
    cvDestroyWindow("GeckoGeek Mask");


}

