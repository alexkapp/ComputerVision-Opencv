#include <stdio.h>
#include <getopt.h>
#include <cmath>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv )
{
	int k = 5;
	int res = 1024;
	Mat A, B;

	int c;
	while ( (c = getopt(argc, argv, "k:r:")) != -1 ) {
		switch(c) {
	
			case 'k':
				k = atoi(optarg);
				break;
			case 'r':
				res = atoi(optarg);
				break;
			default: break;
		}
	}

	A = imread(argv[argc-1], -1);
	if (!A.data) {
		cout << "Invalid Arguments" << endl
			 << " Usage: ./blur image file" << endl
			 << "        ./blur [options] image file" << endl
			 << "[options]: -r <int>: change resolution (default = 1024)" << endl
			 << "           -k <int>: change kernel size (default = 5)" << endl;
		return -1;
	}
	double filter[k][k];
	int radius = k/2; 
	double sum = 0;

	//weight coefficients based on distance from center of kernel 
	for (int x = 0; x < k; x++) 
	for (int y = 0; y < k; y++) {
		filter[x][y] = k / 2 - sqrt( pow(x - radius, 2) + pow(y - radius, 2) ) + 1;
		if (filter[x][y] < 0) filter[x][y] = 0;
		sum += filter[x][y];
	}

	//make the sum of the coefficients equal to the resolution.	
	sum = res / sum;
	for (int x = 0; x < k; x++) 
	for (int y = 0; y < k; y++)
		filter[x][y] *= sum;
	
	//divide coefficients by the resolution so the kernel adds up to 1
	for (int x = 0; x < k; x++) 
	for (int y = 0; y < k; y++) 
		filter[x][y] /= res;

	B = A.clone();

	//run convolution 
	if (A.channels() == 1) {

		for (int x = 0; x < A.cols; x++)
		for (int y = 0; y < A.rows; y++) {

			double pixel = 0.0;

			for (int fx = 0; fx < k; fx++) 
			for (int fy = 0; fy < k; fy++) { 

				int imgX = (x - radius + fx + A.cols) % A.cols;
				int imgY = (y - radius + fy + A.rows) % A.rows;
				pixel += (double) A.at<uchar>(imgY, imgX) * filter[fx][fy];
			}

			B.at<uchar>(y, x) = cvRound(pixel);
		}
	}

	else {
		
		for (int x = 0; x < A.cols; x++)
		for (int y = 0; y < A.rows; y++) {

			double red = 0.0;
			double grn = 0.0;
			double blu = 0.0;
			
			for (int fx = 0; fx < k; fx++) 
			for (int fy = 0; fy < k; fy++) { 

				int imgX = (x - k / 2 + fx + A.cols) % A.cols;
				int imgY = (y - k / 2 + fy + A.rows) % A.rows;
				red += (double) A.at<Vec3b>(imgY, imgX)[0] * filter[fx][fy];
				grn += (double) A.at<Vec3b>(imgY, imgX)[1] * filter[fx][fy];
				blu += (double) A.at<Vec3b>(imgY, imgX)[2] * filter[fx][fy];
			}

			B.at<Vec3b>(y, x)[0] = cvRound(red);
			B.at<Vec3b>(y, x)[1] = cvRound(grn);
			B.at<Vec3b>(y, x)[2] = cvRound(blu);
		}
	}

	namedWindow("Source Image", WINDOW_AUTOSIZE );
   	imshow("Source Image", A);
   	waitKey(0);

    namedWindow("Blur Image", WINDOW_AUTOSIZE );
	imshow("Blur Image", B);
	waitKey(0);

	return 0;
}

