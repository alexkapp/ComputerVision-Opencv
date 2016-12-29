#include <stdio.h>
#include <cmath>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "histogram.h"

using namespace cv;

void equalize(Mat& src) {

	int size = src.size().width * src.size().height;
	
	if (src.channels() == 1) {

		int *cdf = getCDF(src); 
		for (int x = 0; x < src.cols; x++)
		for (int y = 0; y < src.rows; y++) 
				src.at<uchar>(y, x) =  cdf[ src.at<uchar>(y, x) ];
	}
	
	else {

		int *red_cdf = getCDF(src, 2);
		int *grn_cdf = getCDF(src, 1);
		int *blu_cdf = getCDF(src, 0);

		for (int x = 0; x < src.cols; x++)
		for (int y = 0; y < src.rows; y++) {
			src.at<Vec3b>(y, x)[2] = (int) red_cdf[ src.at<Vec3b>(y, x)[2] ];
			src.at<Vec3b>(y, x)[1] = (int) grn_cdf[ src.at<Vec3b>(y, x)[1] ];
			src.at<Vec3b>(y, x)[0] = (int) blu_cdf[ src.at<Vec3b>(y, x)[0] ];
		}
	}
}

void equalize(Mat& src, Mat& dest) {

	dest = src.clone();
	equalize(dest);
}

int * getCDF(Mat src) {

	int * cdf = new int[256];
	int histvals[256] = {0};
	int size = src.size().width * src.size().height;
	

	for (int x = 0; x < src.cols; x++)
	for (int y = 0; y < src.rows; y++) 
		histvals[src.at<uchar>(y, x)]++;

	
	int sum = 0;		
	for (int i = 0; i < 256; i++){
		sum += histvals[i];
		cdf[i] = cvRound(sum * 255 / size + 0.49);
	}
	return cdf;
}

int * getCDF(Mat src, int channel) {

	if (src.channels() == 1)
		return getCDF(src);

	if (channel > 2)
		return NULL;

	int * cdf = new int[256];
	int histvals[256] = {0};
	int size = src.size().width * src.size().height;
	

	for (int x = 0; x < src.cols; x++)
	for (int y = 0; y < src.rows; y++) 
		histvals[ src.at<Vec3b>(y, x)[channel] ]++;

	
	int sum = 0;		
	for (int i = 0; i < 256; i++){
		sum += histvals[i];
		cdf[i] = cvRound(sum * 255 / size + 0.49);
	}
	return cdf;
}
