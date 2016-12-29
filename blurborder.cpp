#include <stdio.h>
#include <getopt.h>
#include <cmath>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int **pmap;
int slider;
int slider_max;
Mat A, B;

void on_trackbar(int, void*) {
		
	for (int x = 0; x < A.cols; x++) 
	for (int y = 0; y < A.rows; y++) {

		if (pmap[x][y] < slider) {

			int dist = pmap[x][y];
	
			int max = A.at<Vec3b>(y, x)[0];
			for (int i = 1; i < 3; i++)
				if (max < A.at<Vec3b>(y, x)[i]) 
					max = A.at<Vec3b>(y, x)[i];
	
			for (int i = 0; i < 3; i++) {
				int p = A.at<Vec3b>(y, x)[i];
				B.at<Vec3b>(y, x)[i] = (double) ((double)(p + ((double)(slider - dist) / slider)*(max - p)));
			}
		}
		else if (pmap[x][y] == slider)
			B.at<Vec3b>(y, x) = {255, 255, 255};

		else
			for (int i = 0; i < 3; i++)
				B.at<Vec3b>(y, x)[i] = A.at<Vec3b>(y, x)[i];
	}
	imshow("Border Blur", B);
}

int main(int argc, char** argv )
{

	if (argc < 2) {
		cout << "No input Image" << endl;
		return -1;
	}

	A = imread(argv[1], CV_LOAD_IMAGE_COLOR);
	if (!A.data) {
		cout << "No image data" << endl;
		return -1;
	}
	int w = A.size().width;
	int h = A.size().height;
	B = A.clone();

	slider = 1;
	slider_max = min(w, h) / 2;

	//create map of pixel distance from edges
	pmap = new int*[w];
	for (int x = 0; x < w; x++) {
		pmap[x] = new int[h]; 
		for (int y = 0; y < h; y++) 
			pmap[x][y] = min( min(x-0, w - x- 1), min(y-0,h - y - 1));
	}
	
	namedWindow("Border Blur", 1);
	createTrackbar("Trackbar", "Border Blur", &slider, slider_max, on_trackbar);
	imshow("Border Blur", B);	
	waitKey(0); 

	return 0;
}
