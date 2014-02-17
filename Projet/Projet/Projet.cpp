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
int h = 10, s = 245, v = 0, tolerance = 10;
 
/*
 * Transform the image into a two colored image, one color for the color we want to track, another color for the others colors
 * From this image, we get two datas : the number of pixel detected, and the center of gravity of these pixel
 */

//CvPoint binarisation(IplImage* image, int *nbPixels){
//
//	
//    int x, y;
//    CvScalar pixel; // element valeur d'un pixel
//    IplImage *hsv, *mask;
//    IplConvKernel *kernel;
//    int sommeX = 0, sommeY = 0;
//    *nbPixels = 0;
//	
//
//
//
//    // Create the mask &initialize it to white (no color detected)
//    mask = cvCreateImage(cvGetSize(image), image->depth, 1); //image->depth = type de donnée de //l'image
// 
//    // Create the hsv image
//    hsv = cvCloneImage(image);
//    cvCvtColor(image, hsv, CV_BGR2HSV);// transforme image d'entrée d'un espace couleur en une //autre
// 
//    // We create the mask
//    cvInRangeS(hsv, cvScalar(h - tolerance -1, s - tolerance, 0), cvScalar(h + tolerance -1, s + tolerance, 255), mask);
// 
//    // Create kernels for the morphological operation --------------- noyau de base (3,3)
//    kernel = cvCreateStructuringElementEx(5, 5, 2, 2, CV_SHAPE_ELLIPSE);
// 
//    // Morphological opening (inverse because we have white pixels on black background)
//    cvDilate(mask, mask, kernel, 1);
//    cvErode(mask, mask, kernel, 1);  
//
//
//}




CvPoint binarisationhsv(IplImage* image, int *nbPixels) {
 
    int x, y;
    CvScalar pixel; // element valeur d'un pixel
    IplImage *hsv, *mask;
    IplConvKernel *kernel;
    int sommeX = 0, sommeY = 0;
    *nbPixels = 0;
	



    // Create the mask &initialize it to white (no color detected)
    mask = cvCreateImage(cvGetSize(image), image->depth, 1); //image->depth = type de donnée de //l'image
 
    // Create the hsv image
    hsv = cvCloneImage(image);
    cvCvtColor(image, hsv, CV_BGR2HSV);// transforme image d'entrée d'un espace couleur en une //autre
 
	// 160-179  0-22 

    // We create the mask
    cvInRangeS(hsv, cvScalar(h - tolerance -1,s-tolerance, 0), cvScalar(h + tolerance -1, s+tolerance, 255), mask);
	 //cvInRangeS(hsv, cvScalar(160, s, 0), cvScalar(179, s , 255), hsv);
   //cvThreshold(hsv,mask,160,179,255);
	 
	 // Create kernels for the morphological operation --------------- noyau de base (3,3)
    kernel = cvCreateStructuringElementEx(5, 5, 2, 2, CV_SHAPE_ELLIPSE);
 
    // Morphological opening (inverse because we have white pixels on black background)
    cvDilate(mask, mask, kernel, 1);
    cvErode(mask, mask, kernel, 1);
	cvErode(mask, mask, kernel, 1);
	cvDilate(mask, mask, kernel, 1);
    
 
    // We go through the mask to look for the tracked object and get its gravity center
    for(x = 0; x < mask->width; x++) {
        for(y = 0; y < mask->height; y++) { 
 
            // If its a tracked pixel, count it to the center of gravity's calcul
            if(((uchar *)(mask->imageData + y*mask->widthStep))[x] == 255) {

                sommeX += x;
                sommeY += y;
                (*nbPixels)++;
            }
        }
    }
 
    // Show the result of the mask image
    cvShowImage("GeckoGeek Mask", mask);
 
    // We release the memory of kernels
    cvReleaseStructuringElement(&kernel);
 
    // We release the memory of the mask
    cvReleaseImage(&mask);
    // We release the memory of the hsv image
        cvReleaseImage(&hsv);
 
    // If there is no pixel, we return a center outside the image, else we return the center of gravity
    if(*nbPixels > 0)
        return cvPoint((int)(sommeX / (*nbPixels)), (int)(sommeY / (*nbPixels)));
    else
        return cvPoint(-1, -1);
}
 
/*
 * Add a circle on the video that fellow your colored object
 */
void addObjectToVideo(IplImage* image, CvPoint objectNextPos, int nbPixels) {
 
    int objectNextStepX, objectNextStepY;
 
    // Calculate circle next position (if there is enough pixels)
    if (nbPixels > 10) {
 
        // Reset position if no pixel were found
        if (objectPos.x == -1 || objectPos.y == -1) {
            objectPos.x = objectNextPos.x;
            objectPos.y = objectNextPos.y;
        }
 
        // Move step by step the object position to the desired position
        if (abs(objectPos.x - objectNextPos.x) > STEP_MIN) {
            objectNextStepX = max(STEP_MIN, min(STEP_MAX, abs(objectPos.x - objectNextPos.x) / 2));
            objectPos.x += (-1) * sign(objectPos.x - objectNextPos.x) * objectNextStepX;
        }
        if (abs(objectPos.y - objectNextPos.y) > STEP_MIN) {
            objectNextStepY = max(STEP_MIN, min(STEP_MAX, abs(objectPos.y - objectNextPos.y) / 2));
            objectPos.y += (-1) * sign(objectPos.y - objectNextPos.y) * objectNextStepY;
        }
 
    // -1 = object isn't within the camera range
    } else {
 
        objectPos.x = -1;
        objectPos.y = -1;
 
    }
 
    // Draw an object (circle) centered on the calculated center of gravity
    if (nbPixels > 10)
        cvDrawCircle(image, objectPos, 15, CV_RGB(0, 0, 0), -1);
 
    // We show the image on the window
    cvShowImage("GeckoGeek Color Tracking", image);
 
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
{
		
	// Touche clavier
    char key=NULL;
    // Image
     IplImage *hsv;
    // Capture vidéo
    CvCapture *capture;
 cout <<"h "<< h <<" s "<<s<<" v "<<v<< endl;
	// Number of tracked pixels
    int nbPixels;
    // Next position of the object we overlay
    CvPoint objectNextPos;

    // Ouvrir le flux vidéo
    //capture = cvCreateFileCapture("/path/to/your/video/test.avi"); // chemin pour un fichier
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
 
    
 
    // Boucle tant que l'utilisateur n'appuie pas sur la touche q (ou Q)
    while(key != 'q' && key != 'Q') {
 
       // On récupère une image
       image = cvQueryFrame(capture);
	
	   // If there is no image, we exit the loop
        if(!image)
            continue;
 
        objectNextPos = binarisationhsv(image, &nbPixels);
        addObjectToVideo(image, objectNextPos, nbPixels);
 

       // On affiche l'image dans une fenêtre
       //cvShowImage( "GeckoGeek Window", hsv);
 
       // On attend 10ms
       key = cvWaitKey(10);
 
    }
 
    cvReleaseCapture(&capture);
	cvDestroyWindow("GeckoGeek Color Tracking");
    cvDestroyWindow("GeckoGeek Mask");
	
	
	
	
	
	
	
	
	
	////------------------------------------------------------------------------------------------------------
	//VideoCapture cap(0); // open the video camera no. 0

	//if (!cap.isOpened())  // if not success, exit program
	//{
	//	cout << "ERROR: Cannot open the video file" << endl;
	//	return -1;
	//}
	//namedWindow("MyVideo",CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"

	//double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
	//double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video

	//cout << "Frame Size = " << dWidth << "x" << dHeight <<endl;
	//Size frameSize(static_cast<int>(dWidth), static_cast<int>(dHeight));

	//VideoWriter oVideoWriter ("MaVideo.avi",CV_FOURCC('P','I','M','1'),20, frameSize, true);
	//	
	//if( !oVideoWriter.isOpened() ) //if not initialize the VideoWriter successfully, exit the program
	//{
	//	cout << "ERROR: Failed to write the video" << endl;
	//	return -1;
	//}

	//
 //   while (1)
 //   {

 //       Mat frame;

	//	bool bSuccess = cap.read(frame); // read a new frame from video

	//	if (!bSuccess) //if not success, break loop
	//	{
	//			cout << "ERROR: Cannot read a frame from video file" << endl;
	//			break;
	//	}

	//	oVideoWriter.write(frame); //writer the frame into the file
	//	imshow("MyVideo", frame); //show the frame in "MyVideo" window

	//	if (waitKey(10) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
	//	{
	//		cout << "esc key is pressed by user" << endl;
	//		break; 
	//	}
 //   }
	//return 0;
}