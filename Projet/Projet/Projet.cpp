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
using namespace std;
using namespace cv; 

 
IplImage *image,*hsv;

// Color tracked and our tolerance towards it
int hR = 10, sR = 245, vR = 0, tolerance = 20;
int hG = 60,sG = 120 ,vG = 0 ;
int hB = 110 ,sB = 210 ,vB = 0;

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
						pile.push(P);
					}
					if(xP-1>=0 && cvGet2D(bin, yP, xP-1).val[0] == 255){
						P.x=xP-1;
						P.y=yP;
						pile.push(P);
					}
					if(yP+1<bin->height && cvGet2D(bin, yP+1, xP).val[0] == 255){
						P.x=xP;
						P.y=yP+1;
						pile.push(P);
					}
					if(yP-1>=0 && cvGet2D(bin, yP-1, xP).val[0] == 255){
						P.x=xP;
						P.y=yP-1;
						pile.push(P);
					}


					if(yP-1>=0 && xP-1>=0 && cvGet2D(bin, yP-1, xP-1).val[0] == 255){
						P.x=xP-1;
						P.y=yP-1;
						pile.push(P);
					}
					if(yP+1<bin->height && xP+1<bin->width && cvGet2D(bin, yP+1, xP+1).val[0] == 255){
						P.x=xP+1;
						P.y=yP+1;
						pile.push(P);
					}
					if(yP+1<bin->height && xP-1>=0 && cvGet2D(bin, yP+1, xP-1).val[0] == 255){
						P.x=xP-1;
						P.y=yP+1;
						pile.push(P);
					}
					if(yP-1>=0 && xP+1<bin->width && cvGet2D(bin, yP-1, xP+1).val[0] == 255){
						P.x=xP+1;
						P.y=yP-1;
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





int main( int argc, const char** argv )
{
	double d, f;
	//debut = clock();

	// Touche clavier
    char key=NULL;
    // Capture vidéo
    CvCapture *capture;
	
    // Ouvrir le flux vidéo
    capture = cvCreateCameraCapture(CV_CAP_ANY);
	
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
		IplImage *imageBis=binarisation(image);
		fin = clock(); 
		cout<<"  binarisation : "<<((double)(fin-debut) / (double) CLOCKS_PER_SEC)<<endl; 
		//debut = clock();
		vector<Centre> tabCentre(detection(imageBis));
		DrawCentre(tabCentre);
		fin = clock(); 
		cout<<"  detection : "<<((double)(fin-debut) / (double) CLOCKS_PER_SEC)<<endl;
		
  
	// Boucle tant que l'utilisateur n'appuie pas sur la touche q (ou Q)
    while(key != 'q' && key != 'Q') {
		 d=clock();
       // On récupère une image
		image = cvQueryFrame(capture);
		hsv = cvCloneImage(image);
		cvCvtColor(image, hsv, CV_BGR2HSV);

		cout<<"                                                    "<<tabCentre.size()<<endl;
		if(tabCentre.size()<6){
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
        if(!image)
            continue;
		
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
       key = cvWaitKey(5);
	   f=clock();
		cout<<"  tt : "<<((double)(f-d) / (double) CLOCKS_PER_SEC)<<endl; 
    }
    cvReleaseCapture(&capture);
	cvDestroyWindow("GeckoGeek Color Tracking");
    cvDestroyWindow("GeckoGeek Mask");


}