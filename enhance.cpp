#include <stdio.h>
#include <cmath>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv )
{
	Mat src, sharp;
	const int k = 3;

	if (argc < 2) {
		cout << "No Image argument" << endl;
		return -1;
	}
	
	src = imread(argv[1], -1);
	if ( !src.data) {
   	    printf("Invalid Input\n");
  	    return -1;
 	}
	
	int lapFilter[k][k] = 
	{	{ 0,  1, 0 },
		{ 1, -4, 1 },
		{ 0,  1, 0 }
	};

	//Run convolution on grayscale image
	if (src.channels() == 1) {

		//initialize shapened image as a signed 2-byte pixel image to increase pixel range
		sharp = Mat::zeros(src.size(), CV_16S);	
		
		for (int x = 0; x < src.cols; x++)
		for (int y = 0; y < src.rows; y++) {

			double pixel = 0.0;

			for (int fx = 0; fx < k; fx++) 
			for (int fy = 0; fy < k; fy++) { 

				int imgX = (x - k / 2 + fx + src.cols) % src.cols;
				int imgY = (y - k / 2 + fy + src.rows) % src.rows;
				pixel += src.at<uchar>(imgY, imgX) * lapFilter[fy][fx];
				
			}
			//short = 2 bytes
			sharp.at<short>(y, x) = pixel;
		}
	
		//Subtract filter image from original
		for (int x = 0; x < src.cols; x++)
		for (int y = 0; y < src.rows; y++)
			sharp.at<short>(y,x) = src.at<uchar>(y, x) - sharp.at<short>(y, x);
	
		//Convert sharpened image to a typical 8-bit, single-channeled grayscale image.	
		sharp.convertTo(sharp, CV_8UC1);
	}

	//run convolution on color image
	else {

		sharp = Mat::zeros(src.size(), src.type());
		
		//create 3 2-byte, single-channel images (1 for each color)
		Mat red = Mat::zeros(src.size(), CV_16S);
		Mat blue = Mat::zeros(src.size(), CV_16S);
		Mat green = Mat::zeros(src.size(), CV_16S);

		for (int x = 0; x < src.cols; x++)
		for (int y = 0; y < src.rows; y++) {

			double redPix = 0.0;
			double grnPix = 0.0;
			double bluPix = 0.0;
			
			for (int fx = 0; fx < k; fx++) 
			for (int fy = 0; fy < k; fy++) { 

				int imgX = (x - k / 2 + fx + src.cols) % src.cols;
				int imgY = (y - k / 2 + fy + src.rows) % src.rows;
				redPix += (double) src.at<Vec3b>(imgY, imgX)[0] * lapFilter[fx][fy];
				grnPix += (double) src.at<Vec3b>(imgY, imgX)[1] * lapFilter[fx][fy];
				bluPix += (double) src.at<Vec3b>(imgY, imgX)[2] * lapFilter[fx][fy];
			}

			//store filtered pixel values in its corresponding Mat
			red.at<short>(y, x) = redPix;
			green.at<short>(y, x) = grnPix;
			blue.at<short>(y, x) = bluPix;
		}

		//subtract filters from original
		for (int x = 0; x < src.cols; x++)
		for (int y = 0; y < src.rows; y++) {
			red.at<short>(y,x) = src.at<Vec3b>(y, x)[0] - red.at<short>(y, x);
			green.at<short>(y,x) = src.at<Vec3b>(y, x)[1] - green.at<short>(y, x);
			blue.at<short>(y,x) = src.at<Vec3b>(y, x)[2] - blue.at<short>(y, x);
		}

		//Convert to unsigned, 8-bit
		red.convertTo(red, CV_8UC1);
		green.convertTo(green, CV_8UC1);
		blue.convertTo(blue, CV_8UC1);

		//create sharpened color image.
		for (int x = 0; x < src.cols; x++)
		for (int y = 0; y < src.rows; y++) {
			sharp.at<Vec3b>(y, x)[0] = red.at<uchar>(y, x);
			sharp.at<Vec3b>(y, x)[1] = green.at<uchar>(y, x);
			sharp.at<Vec3b>(y, x)[2] = blue.at<uchar>(y, x);
		}
	}

	namedWindow("Source Image", WINDOW_AUTOSIZE );
   	imshow("Source Image", src);
	waitKey(0);

	namedWindow("Sharpened Image", WINDOW_AUTOSIZE );
	imshow("Sharpened Image", sharp);
   	waitKey(0);

	return 0;
}
