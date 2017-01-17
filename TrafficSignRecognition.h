
#ifndef _Traffic_H_
#define _Traffic_H_


#include <iostream>
#include <string.h>
#include <stdio.h>
#include "../OpenCV2.1/include/cv.h"
#include "../OpenCV2.1/include/highgui.h"


using namespace cv;

#pragma comment(lib, "OpenCV2.1/lib/cv210.lib")
#pragma comment(lib, "OpenCV2.1/lib/cxcore210.lib")
#pragma comment(lib, "OpenCV2.1/lib/highgui210.lib")

#define AN_MAX_BUFFER 1000

#define NB_OF_CLUSTER 15000

struct mean_var{
	float x_mean, y_mean, variance, var_x, var_y;
};

struct result{
	int match;
};


IplImage* convertPixeltoHSV(IplImage* image);

void detectRed(IplImage* image, IplImage* image_red);

void detectBlue(IplImage* image, IplImage* image_blue);

void detectYellow(IplImage* image, IplImage* image_yellow);

IplImage* cropImage(const IplImage *img, const CvRect region);

float mean_cal(int** arr,int count,int coordinate);

float variance_cal(int** arr,int count,int candidate_count, mean_var *candidate);

int cluster_DFS(IplImage* image, IplImage* image_cluster, int count, int x, int y,int pixel_count, int** arr);

int clustering(IplImage* image, IplImage* image_cluster, mean_var *candidate);



#endif //_Traffic_H_


