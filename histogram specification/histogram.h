/* Histogram Header File */

#include <opencv2/opencv.hpp>
void equalize(cv::Mat&); 
void equalize(cv::Mat&, cv::Mat&); 
int * getCDF(cv::Mat);
int * getCDF(cv::Mat, int);
