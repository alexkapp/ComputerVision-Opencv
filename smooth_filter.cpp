#include <stdio.h>
#include <cmath>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

Mat A, B;
int slider;
const int slider_max = 25;

void runconvolution(Mat, Mat&, int);

void on_trackbar(int, void*) {

	B = A.clone();
	runconvolution(A, B, slider);
	imshow("Kernel Filter", B);
}

int main(int argc, char** argv )
{
	slider = 1;

	A = imread(argv[1], -1);

    if ( !A.data ) {
   	    printf("Invalid Input\n");
  	    return -1;
 	}
	B = A.clone();


	//run filter once with specified kernel size
	if (argc == 3) {

		int k = atoi(argv[2]);
		if (k % 2 == 0) k++;

		runconvolution(A, B, k);
		char name[20];
		sprintf(name, "Filter: %d X %d", k, k);
		namedWindow(name, 1);
		imshow(name, B);
		waitKey(0);
	}

	//Use trackbar to specify kernel size	
	else {

		namedWindow("Kernel Filter", 1);
		createTrackbar("Trackbar: k x k", "Kernel Filter", &slider, slider_max, on_trackbar);	
		imshow("Kernel Filter", B);
		waitKey(0);
	}

	return 0;
}

void runconvolution(Mat src, Mat& dest, int k) {

	double filter[k][k]; 
	for (int i = 0; i < k; i++) 
	for (int j = 0; j < k; j++) 
		filter[i][j] = (double) 1 / (k * k);

	if (src.channels() == 1) {

		for (int x = 0; x < src.cols; x++)
		for (int y = 0; y < src.rows; y++) {

			double pixel = 0.0;

			for (int fx = 0; fx < k; fx++) 
			for (int fy = 0; fy < k; fy++) { 

				int imgX = (x - k / 2 + fx + src.cols) % src.cols;
				int imgY = (y - k / 2 + fy + src.rows) % src.rows;
				pixel += (double) src.at<uchar>(imgY, imgX) * filter[fx][fy];
			}	

			dest.at<uchar>(y, x) = cvRound(pixel);
		}	
	}

	else {
		for (int x = 0; x < src.cols; x++)
		for (int y = 0; y < src.rows; y++) {

			double red = 0.0;
			double grn = 0.0;
			double blu = 0.0;
		
			for (int fx = 0; fx < k; fx++) 
			for (int fy = 0; fy < k; fy++) { 

				int imgX = (x - k / 2 + fx + src.cols) % src.cols;
				int imgY = (y - k / 2 + fy + src.rows) % src.rows;
				red += (double) src.at<Vec3b>(imgY, imgX)[0] * filter[fy][fx];
				grn += (double) src.at<Vec3b>(imgY, imgX)[1] * filter[fy][fx];
				blu += (double) src.at<Vec3b>(imgY, imgX)[2] * filter[fy][fx];
			}

			dest.at<Vec3b>(y, x)[0] = cvRound(red);
			dest.at<Vec3b>(y, x)[1] = cvRound(grn);
			dest.at<Vec3b>(y, x)[2] = cvRound(blu);
		}
	}
}
