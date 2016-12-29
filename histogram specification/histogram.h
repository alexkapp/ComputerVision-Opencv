#include <opencv2/opencv.hpp>
//#ifndef HISTOGRAM_H
//#define HISTOGRAM_H
void equalize(cv::Mat&); 
void equalize(cv::Mat&, cv::Mat&); 
int * getCDF(cv::Mat);
int * getCDF(cv::Mat, int);
//#endif
