#include <stdio.h>
#include <cmath>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <getopt.h>
#include <fstream>
#include "histogram.h"

using namespace cv;
using namespace std;

void matchrefimage(char * , char *);
void matchreffile(char * , char *);

int main(int argc, char** argv )
{
	char reffile[50];
	char imagefile[50];

	if (argc < 4) {
		cout << "Usage: " << argv[0] << " [-f] [-i] [image path...] [ref path...]" << endl
			 << "\t[-i] ref = specified hist file is image" << endl
			 << "\t[-f] ref = specidied hist file is a list of ints" << endl;
		return 0;	
	}

	//Process commandline arguments
	int c;	
	while ( (c = getopt(argc, argv, "hi:f:")) != -1 ) {
		
		switch(c) {
			
			case 'h':

				cout << "Usage: " << argv[0] << " [-f] [-i] [image path...] [ref path...]" << endl
					 << "\t[-i] ref = specified hist file is image" << endl
					 << "\t[-f] ref = specidied hist file is a list of ints" << endl;
					return 0;	
			break;

			case 'i':
				strcpy(imagefile, optarg);
				strcpy(reffile, argv[argc-1]);

				//run histogram specification
				matchrefimage(imagefile, reffile);
			break;

			case 'f':
				strcpy(imagefile, optarg);
				strcpy(reffile, argv[argc-1]);

				//run histogram specification 
				matchreffile(imagefile, reffile);
			break;
			
			default:
			break;
		}
	}
	return 0;
}

//Target histogram is an image
void matchrefimage(char *src, char *trgt) {

	Mat target = imread(trgt, CV_8UC1);
	Mat source = imread(src, CV_8UC1);
	Mat result = source.clone(); 
	
	if (!target.data || !source.data)
		printf("Unable to open image files\n");

	int *targt_cdf = getCDF(target); 
	int *src_cdf = getCDF(source);

	int map[256] = {0};

	//map pixels
	for (int i = 0; i < 256; i++) {
		int j;
		for (j = 0;(targt_cdf[j] - src_cdf[i]) < 0; j++); 
		map[i] = j;
	}

	//update result image
	for (int x = 0; x < source.cols; x++)
	for (int y = 0; y < source.rows; y++)
		result.at<uchar>(y, x) =  map[source.at<uchar>(y, x)];

	//Display Images
	namedWindow("source image", WINDOW_AUTOSIZE );
	imshow("source image", source);
	waitKey(0);
	namedWindow("ref image", WINDOW_AUTOSIZE );
	imshow("ref image", target);
	waitKey(0);
	namedWindow("Match Result", WINDOW_AUTOSIZE );
	imshow("Match Result", result);
	waitKey(0);
}

//Target histogram comes from file of ints
void matchreffile(char *src, char *trgt) {

	ifstream target(trgt);
	Mat source = imread(src, CV_8UC1);
	Mat result = source.clone(); 
	
	if (!source.data) {
		printf("Unable to open image files\n");
		return;
	}

	int targt_hist[256] = {0};
	int targt_cdf[256] = {0};
	int *src_cdf;
	
	int map[256] = {0};

	int tSum = 0;
	int tsize = 0;

	string line;

	//update target histogram by reading from file
	while ( getline(target, line)) {
	//	int n = atoi(line.c_str());
	//	targt_hist[n]++;
		targt_hist[ atoi(line.c_str()) ]++;	
		tsize++;
	}

	//get targets cdf	
	for (int i = 0; i < 256; i++) {
		tSum += targt_hist[i];
		targt_cdf[i] = cvRound( tSum * 255 / tsize + 0.49);
	}
	src_cdf = getCDF(source);

	//get pixel map 
	for (int i = 0; i < 256; i++) {
		int j;
		for (j = 0;(targt_cdf[j] - src_cdf[i]) < 0; j++); 
		map[i] = j;
	}

	//update resulting image
	for (int x = 0; x < source.cols; x++)
	for (int y = 0; y < source.rows; y++)
		result.at<uchar>(y, x) =  map[source.at<uchar>(y, x)];

	//display result
	namedWindow("Match Result", WINDOW_AUTOSIZE );
    imshow("Match Result", result);
	waitKey(0);
}
