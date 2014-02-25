#include "opencv2/core/core.hpp"
#include "opencv2/flann/miniflann.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/photo/photo.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/ml/ml.hpp"
#include "opencv2/contrib/contrib.hpp"

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"



#include <stdlib.h>
#include <stdio.h>

#include "stdafx.h"
#include "opencv2/highgui/highgui.hpp"
//Pensée dans les propriétés du project a prendre les bonne librairie pour le debug et l'autre pour le releases

#include<cv.h>
#include <iostream>
#include <time.h>

using namespace std;
using namespace cv; 



// Maths methods
#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))  
#define abs(x) ((x) > 0 ? (x) : -(x))
#define sign(x) ((x) > 0 ? 1 : -1)
 
// Step mooving for object min & max
#define STEP_MIN 5
#define STEP_MAX 100 
 
IplImage *image;
 
// Position of the object we overlay
CvPoint objectPos = cvPoint(-1, -1);
// Color tracked and our tolerance towards it
int hR = 10, sR = 245, vR = 0, tolerance = 20;
int hG = 60,sG = 160 ,vG = 0 ;
int hB = 110 ,sB = 210 ,vB = 0;

CvMemStorage* storage = cvCreateMemStorage(0);
double debut, fin; 

void binarisation(IplImage* image, int *nbPixels);
void detection(IplImage* mask);
void Inverse();

/*
 * Transform the image into a two colored image, one color for the color we want to track, another color for the others colors
 */
void binarisation(IplImage* image, int *nbPixels) {
 
    int x, y;
    CvScalar pixel; // element valeur d'un pixel
    IplImage *hsv, *mask;
    IplConvKernel *kernel;
	
    // Create the mask &initialize it to white (no color detected)
    mask = cvCreateImage(cvGetSize(image), IPL_DEPTH_8U, 1); //image->depth = type de donnée de //l'image

    // Create the hsv image
    hsv = cvCloneImage(image);
    cvCvtColor(image, hsv, CV_BGR2HSV);// transforme image d'entrée d'un espace couleur en une //autre
 
    // We create the mask
	 for (int x=0;x<hsv->width; x++)
    {
        for (int y=0; y<hsv->height; y++)
        {
            pixel=cvGet2D(hsv, y, x);
            if ( ((pixel.val[0]>=hR-tolerance && pixel.val[1]>=sR-tolerance)&&(pixel.val[0]<=hR+tolerance && pixel.val[1]<=sR+tolerance))
				||((pixel.val[0]>=hB-tolerance && pixel.val[1]>=sB-tolerance)&&(pixel.val[0]<=hB+tolerance && pixel.val[1]<=sB+tolerance))
				||((pixel.val[0]>=hG-tolerance && pixel.val[1]>=sG-tolerance)&&(pixel.val[0]<=hG+tolerance && pixel.val[1]<=sG+tolerance)) )
            {  pixel.val[0]=255; cvSet2D(mask, y, x,pixel);
            }
            else {
                pixel.val[0]=0; cvSet2D(mask, y, x,pixel);
			}
			
        }
    }
	  // We release the memory of the hsv image
        cvReleaseImage(&hsv);




	// Create kernels for the morphological operation --------------- noyau de base (3,3)
    kernel = cvCreateStructuringElementEx(10, 10, 4, 4, CV_SHAPE_ELLIPSE);
 
    // Morphological opening and closing to remove the holes and the little object
    cvDilate(mask, mask, kernel, 1);
	//cvDilate(mask, mask, kernel, 1);
	//cvDilate(mask, mask, kernel, 1);
    cvErode(mask, mask, kernel, 1);
	//cvErode(mask, mask, kernel, 1);
	//cvErode(mask, mask, kernel, 1);
	//cvErode(mask, mask, kernel, 1);
	//cvErode(mask, mask, kernel, 1);
	cvErode(mask, mask, kernel, 1);
	//cvDilate(mask, mask, kernel, 1);
	cvDilate(mask, mask, kernel, 1);
	//cvDilate(mask, mask, kernel, 1);
	 // We release the memory of kernels
    cvReleaseStructuringElement(&kernel);


	

	debut = clock();
    detection(mask);
	fin = clock(); 
	cout<<"detection       temps : "<<((double)(fin-debut) / (double) CLOCKS_PER_SEC)<<endl; 


    // Show the result of the mask image
    cvShowImage("GeckoGeek Mask", mask);

    // We release the memory of the mask
    cvReleaseImage(&mask);
 
}
 
void Inverse(){
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
			//ImgInv->imageData[ y*ImgInv->widthStep+ x*3] = (int)&pixel; 
		}
	 }
	  //cvShowImage("ImgInv", ImgInv);
	 image=cvCloneImage(ImgInv);
	 cvReleaseImage(&ImgInv);
}




void detection(IplImage* mask){

	IplImage* canny = cvCreateImage(cvGetSize(image),IPL_DEPTH_8U,1);
    IplImage* rgbcanny = cvCreateImage(cvGetSize(image),IPL_DEPTH_8U,3); // 3 et 1 difference
    cvCanny(mask, canny, 0, 500, 3);   // arguments???

    CvSeq* circles = cvHoughCircles(mask, storage, CV_HOUGH_GRADIENT, 1, 30, 10.0, 5.0,5,30);
    cvCvtColor(canny, rgbcanny, CV_GRAY2BGR);
	
	//cout<<circles->total<<endl;
    for (size_t i = 0; i < circles->total; i++)
    {
         // round the floats to an int
         float* p = (float*)cvGetSeqElem(circles, i);
         cv::Point center(cvRound(p[0]), cvRound(p[1]));
         int radius = cvRound(p[2]);

         // draw the circle center
         cvCircle(rgbcanny, center, 3, CV_RGB(0,255,0), -1, 8, 0 );

         // draw the circle outline
         cvCircle(rgbcanny, center, radius+1, CV_RGB(0,0,255), 2, 8, 0 );
		
         //cout<<"x: "<<center.x<<" y: "<<center.y<<" r: "<<radius<<endl;
    }

	
	cvNamedWindow("circles", 1);
    cvShowImage("circles", rgbcanny);
	cvReleaseImage(&rgbcanny);

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
        pixel = cvGet2D(hsv, y, x);
 
		cout <<"h "<< (int)pixel.val[0] <<" s "<<(int)pixel.val[1]<<" v "<<(int)pixel.val[2]<< endl;
        // Release the memory of the hsv image
            cvReleaseImage(&hsv);
 
    }
 
}


int main( int argc, const char** argv )
{double d, f;
	
	debut = clock();

	// Touche clavier
    char key=NULL;
    // Image
     IplImage *hsv;
    // Capture vidéo
    CvCapture *capture;
	
	// Number of tracked pixels
    int nbPixels;
    // Next position of the object we overlay
    CvPoint objectNextPos;

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
 
    fin = clock(); 
		cout<<"initialisation       temps : "<<((double)(fin-debut) / (double) CLOCKS_PER_SEC)<<endl; 
 
    // Boucle tant que l'utilisateur n'appuie pas sur la touche q (ou Q)
    while(key != 'q' && key != 'Q') {
 d=clock();
       // On récupère une image
		debut = clock();
		image = cvQueryFrame(capture);
		fin = clock(); 
		cout<<"capture       temps : "<<((double)(fin-debut) / (double) CLOCKS_PER_SEC)<<endl; 
		/*debut = clock();
		Inverse();
		fin = clock(); 
		cout<<"inverse       temps : "<<((double)(fin-debut) / (double) CLOCKS_PER_SEC)<<endl; */
	   // If there is no image, we exit the loop
        if(!image)
            continue;
 
		 // Start timer
		debut = clock();
		binarisation(image, &nbPixels);
		fin = clock(); 
		cout<<"  binarisation : "<<((double)(fin-debut) / (double) CLOCKS_PER_SEC)<<endl; 


		//addObjectToVideo(image, objectNextPos, nbPixels);
		cvShowImage("GeckoGeek Color Tracking", image);

       // On attend 10ms
       key = cvWaitKey(5);
	   f=clock();
		cout<<"  tt : "<<((double)(f-d) / (double) CLOCKS_PER_SEC)<<endl; 
    }
 
    cvReleaseCapture(&capture);
	cvDestroyWindow("GeckoGeek Color Tracking");
    cvDestroyWindow("GeckoGeek MaskR");
	

//return 0;
}