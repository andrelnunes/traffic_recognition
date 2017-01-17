#include "../header/TrafficSignRecognition.h"

int main()
{
	char first[255];
	char last[255];
	char full[255];

	//number of images to check
	int num =55;
	//result r[10];
	//setting string to receive all the names in the form: "(Test)(number)(.jpg)"
	//sprintf_s(first,  "data");
	sprintf_s(last,  ".jpg");
	//sprintf_s(full,  "%d%s", num, last);
	
	for (int k=1; k<num; k++)
	{
		sprintf_s(full,  "%d%s", k, last);
		
		IplImage* image = cvLoadImage(full, CV_LOAD_IMAGE_GRAYSCALE);      

		cvSmooth(image, image, CV_GAUSSIAN,3,3);
		cvDilate(image,image,NULL, 1);

		//cvCvtColor(image, image, CV_GRAY2RGB);
		/*IplImage* hsv = cvCreateImage( cvGetSize(image), IPL_DEPTH_8U, 3 );
		IplImage *gray = cvCreateImage(cvGetSize(image),IPL_DEPTH_8U,1);
		
		hsv = convertPixeltoHSV(image);		
		CvMemStorage* storage = cvCreateMemStorage(0);
		cvCvtColor(image, gray, CV_RGB2GRAY);

		IplImage* image_red = cvCreateImage( cvGetSize(image), IPL_DEPTH_8U, 1 );
		IplImage* image_blue = cvCreateImage( cvGetSize(image), IPL_DEPTH_8U, 1 );
		IplImage* image_yellow = cvCreateImage( cvGetSize(image), IPL_DEPTH_8U, 1 );
		IplImage *image_gray = cvCreateImage(cvGetSize(image),IPL_DEPTH_8U,1);

		cvSmooth(hsv, hsv, CV_GAUSSIAN,3,3);
		
		detectRed(hsv, image_red);
		detectBlue(hsv, image_blue);
		detectYellow(hsv, image_yellow);

		// Add the tracking image and the frame	
		cvAdd(image_red, image_blue, image_gray);
		cvAdd(image_gray, image_yellow, image_gray);

		cvDilate(image_gray,image_gray,NULL, 1);
		cvDilate(image_gray,image_gray,NULL, 1);
		cvDilate(image_gray,image_gray,NULL, 1);
		cvDilate(image_gray,image_gray,NULL, 1);
		//cvErode(image_gray,image_gray,NULL, 1);
		

		cvNamedWindow( "viewer_gray", 1);
		cvShowImage( "viewer_gray", image_gray);      // show image
		cvWaitKey(0);
			
		IplImage* image_cluster = cvCreateImage( cvGetSize(image), IPL_DEPTH_8U, 1 );
		
		mean_var candidates[50];
		int can=0;
		can = clustering(image_gray, image_cluster, candidates);

		cvNamedWindow( "viewer_2", 1);
		cvShowImage( "viewer_2", image_cluster);      // show image
		cvWaitKey(0);

	CvRect region;
	
	for(int i=0; i<5; i++){

		//region = cvRect( candidates[i].x_mean-(candidates[i].x_mean/2), candidates[i].y_mean-(candidates[i].y_mean/2), candidates[i].x_mean, candidates[i].y_mean );
		if( (candidates[i].x_mean-75)>0 && (candidates[i].y_mean-75)>0){region= cvRect( candidates[i].x_mean-75, candidates[i].y_mean-75, 180, 180 );}
		else{
			if((candidates[i].y_mean-75)<0 && (candidates[i].x_mean-75)<0){region= cvRect( 0, 0, 180, 180 );}
			else{
				if((candidates[i].y_mean-75)<0 && (candidates[i].y_mean-75)!=0) {region= cvRect(  candidates[i].x_mean-75, 0, 180, 180 );}
				else
					if((candidates[i].x_mean-75)<0) {region= cvRect( 0, candidates[i].y_mean-75, 180, 180 );}
			}
		}
			//cutting image
			cropImage(image_cluster, region);
			cropImage(gray, region);

			//use cvResize to resize source to a destination image
			CvSize size= cvSize(64,64 );
			IplImage *destination = cvCreateImage ( size, IPL_DEPTH_8U, 1 );
			IplImage *destinationColor = cvCreateImage ( size, IPL_DEPTH_8U, 1 );

			cvResize(image_cluster, destination);
			cvResize(gray, destinationColor);*/

			sprintf_s(full,  "%d%s", k+54, last);
			cvSaveImage( full, image);

			/*( "viewerDest", 1);
			cvShowImage( "viewerDest", destinationColor);      // show image
			cvWaitKey(0);*/

	}
		//sprintf_s(full,  "data%d%s", k, last);
		//cvSaveImage( full, gray );

		//cvReleaseImage(&image);              // release memory allocation
		//cvReleaseImage(&image_gray);
		cvWaitKey(0);
	//}

	return 1;
}

IplImage* convertPixeltoHSV(IplImage* image)
{	
	IplImage* hsv = cvCreateImage( cvGetSize(image), IPL_DEPTH_8U, 3 );

	//Iterate for each pixel
	for(int i=0;i<image->height;i++)
		for(int j=0;j<image->width;j++)
		{
			unsigned char B = image->imageData[ ( i*image->widthStep + j*image->nChannels) ];
			unsigned char G = image->imageData[  i*image->widthStep + j*image->nChannels + 1];
			unsigned char R = image->imageData[  i*image->widthStep + j*image->nChannels + 2];

			unsigned char M =  max(B,max(G,R));
			unsigned char m =  min(B,min(G,R));
			unsigned char C = M - m;

			float H_prime;

			if(C == 0)
				H_prime = 0;
			else if( M == R)
				H_prime = ((G-B)/(float)C);
			else if (M == G)
				H_prime = (B-R)/((float)C) + 2;
			else if (M == B)
				H_prime = (R-G)/((float)C) + 4;

		 
			hsv->imageData[ ( i*image->widthStep + j*image->nChannels) ] = (60*H_prime);	//Hue
			hsv->imageData[ ( i*image->widthStep + j*image->nChannels) + 1] = (C/((float) M))*255;		//Saturation
			hsv->imageData[ ( i*image->widthStep + j*image->nChannels) + 2] = M;		//Value
		}
	return hsv;
}

void detectRed(IplImage* image, IplImage* image_red)
{	
	//Iterate for each pixel
	for(int i=0;i<image->height;i++)
		for(int j=0;j<image->width;j++)
		{
			float Hue = (unsigned char) image->imageData[ ( i*image->widthStep + j*image->nChannels) ];
			Hue = Hue/360;

			float Saturation = (unsigned char) image->imageData[ ( i*image->widthStep + j*image->nChannels) + 1 ];
			Saturation = Saturation/255;

			float Value = (unsigned char) image->imageData[ ( i*image->widthStep + j*image->nChannels) + 2 ];
			Value = Value/255;

			if ((((Hue >= 0.68) || (Hue<=0.1)) && (Saturation >= 0.5 && Saturation <=1) && (Value >= 0.5 && Value <=1)))
			{
				image_red->imageData[ ( i*image_red->widthStep + j )+2 ] = 255;
			}
			else
			{
				image_red->imageData[ ( i*image_red->widthStep + j)+2 ] = 0;
			}
		}
}

void detectBlue(IplImage* image, IplImage* image_blue)
{	
	//Iterate for each pixel
	for(int i=0;i<image->height;i++)
		for(int j=0;j<image->width;j++)
		{
			float Hue = (unsigned char) image->imageData[ ( i*image->widthStep + j*image->nChannels) ];
			Hue = Hue/360;

			float Saturation = (unsigned char) image->imageData[ ( i*image->widthStep + j*image->nChannels) + 1 ];
			Saturation = Saturation/255;

			float Value = (unsigned char) image->imageData[ ( i*image->widthStep + j*image->nChannels) + 2 ];
			Value = Value/255;

			if (((Hue >= 0.55  && Hue <= 0.65) && (Saturation >= 0.5 && Saturation <=0.95) && (Value >= 0.5 && Value <= 6)))
			{
				image_blue->imageData[ ( i*image_blue->widthStep + j )+2 ] = 255;
			}
			else
			{
				image_blue->imageData[ ( i*image_blue->widthStep + j)+2 ] = 0;
			}
		}
}

void detectYellow(IplImage* image, IplImage* image_yellow)
{	
	//Iterate for each pixel
	for(int i=0;i<image->height;i++)
		for(int j=0;j<image->width;j++)
			{
			float Hue = (unsigned char) image->imageData[ ( i*image->widthStep + j*image->nChannels) ];
			Hue = Hue/360;

			float Saturation = (unsigned char) image->imageData[ ( i*image->widthStep + j*image->nChannels) + 1 ];
			Saturation = Saturation/255;

			float Value = (unsigned char) image->imageData[ ( i*image->widthStep + j*image->nChannels) + 2 ];
			Value = Value/255;

			if (((Hue >= 0.1  && Hue <= 0.2) && (Saturation >= 0.55 && Saturation <=0.8) && (Value >= 0.5 && Value <= 1)))
			{
				image_yellow->imageData[ ( i*image_yellow->widthStep + j )+2 ] = 255;
			}
			else
			{
				image_yellow->imageData[ ( i*image_yellow->widthStep + j)+2 ] = 0;
			}
		}
}

IplImage* cropImage(const IplImage *img, const CvRect region)
{
	IplImage *imageCropped;
	CvSize size;

	if (img->width <= 0 || img->height <= 0 || region.width <= 0 || region.height <= 0) {
		//cerr << "ERROR in cropImage(): invalid dimensions." << endl;
		exit(1);
	}
	
	if (img->depth != IPL_DEPTH_8U) {
		//cerr << "ERROR in cropImage(): image depth is not 8." << endl;
		exit(1);
	}

	// Set the desired region of interest.
	cvSetImageROI((IplImage*)img, region);
	// Copy region of interest into a new iplImage and return it.
	size.width = region.width;
	size.height = region.height;
	imageCropped = cvCreateImage(size, IPL_DEPTH_8U, img->nChannels);
	cvCopy(img, imageCropped);	// Copy just the region.

	return imageCropped;
}

float mean_cal(int** arr,int count,int coordinate)
{
	int i = 0;
	float temp = 0;

	while (i < count)
	{
		temp = temp + arr[i][coordinate];
		i++;
	}

	return temp/count;
}

float variance_cal(int** arr,int count,int candidate_count, mean_var *candidate)
{
	float temp_x = 0;
	float temp_y = 0;
		int i = 0;

	float x_mean = mean_cal(arr,count,0);
	float y_mean = mean_cal(arr,count,1);
	float max_y = 0, max_x =0;

	while( i < count)
	{
		temp_x = temp_x + (arr[i][0] - x_mean)*(arr[i][0] - x_mean);
		temp_y = temp_y + (arr[i][1] - y_mean)*(arr[i][1] - y_mean);
		i++;
	}

	float var_x = temp_x/(float)count;
	float var_y = temp_y/(float)count;

	std::cout << count << " " << abs(var_x - var_y)/count << " " << abs(var_x - var_y) << " (" << var_x << "," << var_y << ")" << std::endl;

		candidate[candidate_count].x_mean = x_mean;
		candidate[candidate_count].y_mean = y_mean;
		candidate[candidate_count].var_x = var_x;
		candidate[candidate_count].var_y = var_y;
		candidate[candidate_count].variance = (var_x+var_y)/2;
		return abs(var_x - var_y);

		return 0;
}

int cluster_DFS(IplImage* image, IplImage* image_cluster, int count, int x, int y,int pixel_count, int** arr)
{
	if (pixel_count >= 1000)
	{
		return pixel_count;
	}
	for (int local_y = -1; local_y <= 1; local_y++)
		for (int local_x = -1; local_x <= 1; local_x++)
		{
			if(( (y + local_y) > 0 && (y + local_y < image->height)) && ((x + local_x) > 0 && (x + local_x < image->height)) )
			{
				if((image->imageData[(y+local_y)*image->widthStep + x + local_x] != 0) && (image_cluster->imageData[(y+local_y)*image->widthStep + x + local_x] == 0))
				{
					image_cluster->imageData[(y+local_y)*image->widthStep + x + local_x]= count;
					arr[pixel_count][0] = x+local_x;
					arr[pixel_count][1] = y+local_y;
					pixel_count++;
					int temp = cluster_DFS(image,image_cluster,count,x+local_x,y+local_y,pixel_count,arr);
					pixel_count = temp;

				}
			}
		}

	return pixel_count;
}

int clustering(IplImage* image, IplImage* image_cluster, mean_var *candidate)
{
	int count = 1,candidate_count = 0;

	IplImage* temp_im = cvCreateImage( cvGetSize(image), IPL_DEPTH_8U, 1 );

	for(int y=0;y<image->height;y++)
		for(int x=0;x<image->width;x++)
		{
			image_cluster->imageData[y*image->widthStep + x] = 0;
			temp_im->imageData[y*image->widthStep + x] = 0;
		}

	for(int y=0;y<image->height;y++)
		for(int x=0;x<image->width;x++)
		{
			if((image->imageData[y*image->widthStep + x] != 0) && (temp_im->imageData[y*image->widthStep + x] == 0))
			{
				//int arr[10000][2] = {0};

				int** arr	= new int*[10000];
				for(int i = 0 ; i < 10000 ; i++)
				{
					arr[i] = new int[2];
				}
				for(int i = 0 ; i < 10000 ; i++)
				{
					for(int j = 0; j < 2; j++)
					{
						arr[i][j] = 0;
					}
				}				

				temp_im->imageData[y*image->widthStep + x] = count;
				arr[0][0]=x;
				arr[0][1]=y;
				int count1 = cluster_DFS(image, temp_im, count, x, y, 1, arr);
				
				if ((count1 >= 1000) && (count1 <= NB_OF_CLUSTER))
				{
					float temp = 0;
					temp = variance_cal(arr,count1,candidate_count,candidate);

					for( int i = 0 ; i < count1 ; i++)
					{
						image_cluster->imageData[arr[i][1]*image->widthStep + arr[i][0]] = 255;
					}
					if (temp != 0)
					{						
						candidate_count++;
					}
				}
				count++;
			}
		}
	
	cvReleaseImage(&temp_im);
	std::cout << std::endl;

	if (candidate_count != 0)
		return candidate_count;
	else
		return 0;
}
