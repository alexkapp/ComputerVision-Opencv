#include <stdio.h>
#include <getopt.h>
#include <cmath>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void onMouse(int, int, int, int, void*);
Mat zoom(Mat, int, int, int, int, int);
Mat A, B, Z; 
Rect rect(0,0,0,0);
Point p1(0,0);
Point p2(0,0);
bool clicked = false;
int z = 5;

void showImage() {

	if (rect.width > 0 && rect.height > 0 && !clicked) {
		int r = rect.y + (rect.height / 2);
		int c = rect.x + (rect.width / 2);
		
		Z = zoom(A, z, r, c, rect.width, rect.height);
		destroyWindow("Source Image");
		namedWindow("Zoomed Image", WINDOW_AUTOSIZE); 
		imshow("Zoomed Image", Z);
		waitKey(0);
	}
	else {		
		B = A.clone();
		rectangle(B, rect, Scalar(0,0,0), 1, 8, 0);
		imshow("Source Image", B);	
	}
}

int main(int argc, char** argv )
{
	if (strcmp(argv[1], "-z") == 0 && isdigit(*argv[2])) {
		z = atoi(argv[2]);
		A = imread(argv[3], -1);
	}
	else 
		A = imread(argv[1], -1);
	
	if (!A.data) {

		cout << "Usage:" << endl;
		cout << "       ./zoom -z [int] <Image_Path>" << endl;
		cout << "       ./zoom <Image_Path>" << endl;
		return -1;
	}

	B = A.clone();
	namedWindow("Source Image", WINDOW_AUTOSIZE);
	setMouseCallback("Source Image", onMouse, NULL);
	imshow("Source Image", B);
	waitKey(0);
		
	return 0;
}

Mat zoom(Mat A, int z, int R, int C, int W, int H) {

	Mat zoomed = A.clone();
	int zx, zy;
	
	//Grayscale Image	
	if ( A.channels() == 1)
		for (int x = R - (H / 2); x < R + (H / 2); x++) 
		for (int y = C - (W / 2); y < C + (W / 2); y++) {
			zx = (x - R <= 0) ? (x - R) / z : ((x - R) / z) + 1;  
			zy = (y - C <= 0) ? (y - C) / z : ((y - C) / z) + 1;
			zoomed.at<uchar>(x, y) = A.at<uchar>(R + zx, C + zy);
		}
	
	//Color Image
	else
		for (int x = R - (H / 2); x < R + (H / 2); x++) 
		for (int y = C - (W / 2); y < C + (W / 2); y++) { 
			zx = (x - R <= 0) ? (x - R) / z : ((x - R) / z) + 1;  
			zy = (y - C <= 0) ? (y - C) / z : ((y - C) / z) + 1;
				
			for (int i = 0; i < 3; i++) 
				zoomed.at<Vec3b>(x, y)[i] = A.at<Vec3b>(R + zx, C + zy)[i];
		}
	return zoomed;
}

void onMouse(int event, int x, int y, int f, void*) {

	switch(event) {

		case CV_EVENT_LBUTTONDOWN :
									clicked = true;
									p1.x = x;
									p1.y = y;
									p2.x = x;
									p2.y = y;
									break;

		case CV_EVENT_LBUTTONUP	  : p2.x = x;
								    p2.y = y;
								    clicked = false;
								    break;

		case CV_EVENT_MOUSEMOVE   :
									if (clicked) {
										p2.x = x;
										p2.y = y;
									}
									break;

		default:
			break;
	}
	
	if (clicked) {
		rect.x = ( (p1.x > p2.x) ? p2.x : p1.x );
		rect.y = ( (p1.y > p2.y) ? p2.y : p1.y );
		rect.width =  ( (p1.x > p2.x) ? p1.x - p2.x : p2.x - p1.x );	
		rect.height = ( (p1.y > p2.y) ? p1.y - p2.y : p2.y - p1.y );
	}
	showImage();
}
