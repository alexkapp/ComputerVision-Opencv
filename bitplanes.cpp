#include <stdio.h>
#include <unistd.h>
#include <cmath>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;
int main(int argc, char** argv )
{
    if ( argc != 2 ) {
        cout << "usage: ./bitplanes <Image_Path>" << endl;
        return -1;
    }

    Mat image = imread( argv[1], CV_8UC1);

    if ( !image.data ) {
        printf("No image data \n");
        return -1;
    }

	Mat planes[8];
	//Initialize bit Plane Images 
	for (int i = 0; i < 8; i++) 
		planes[i] = Mat::zeros(image.size(), image.type());

	//Store input image into seperated bit planes
	for (int x = 0; x < image.rows; x++) 
	for (int y = 0; y < image.cols; y++) 
	for (int i = 0; i < 8; i++)	{
		int p = pow(2, i); 
		planes[i].at<uchar>(x, y) = ( (image.at<uchar>(x,y) & p) / p ) * 255;
	} 
		
	//Display Input and bit plane images
	namedWindow("Input Image", CV_WINDOW_AUTOSIZE);
	imshow("Input Image", image);
	waitKey(0);

	for (int i = 0; i < 8; i++) {
		char name[12];
		sprintf(name, "Plane %d", i);

		namedWindow(name, CV_WINDOW_AUTOSIZE);
		imshow(name, planes[i]);
		waitKey(0);
   	} 

    return 0;
}
