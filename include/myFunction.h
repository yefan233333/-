#pragma once

#include<iostream>
#include<opencv2/opencv.hpp>
#include<vector>
#include"myLine.h"

// int contour_min_area = 350;
extern const cv::Scalar contours_scalar;
extern const int contours_thickness;

cv::Point2f getPointsCenter(std::vector<cv::Point> points);
double getPointDistance(cv::Point p1, cv::Point p2);
double getPointDistance(cv::Point2f p1, cv::Point2f p2);

void drawPoints(cv::Mat src,std::vector<cv::Point> points,cv::Scalar color);
void drawPoints(cv::Mat src,std::vector<cv::Point2f> points,cv::Scalar color);
void drawPoints(cv::Mat src,cv::Point2f points[],int len,cv::Scalar color);
cv::Point2f getPointsAverage(std::vector<cv::Point2f> points);
cv::Point2f getPointsVar(std::vector<cv::Point2f> points);
cv::Point2f getCircleCenter(cv::Point2f p1,cv::Point2f p2,cv::Point2f p3);
std::vector<double> generateGaussianKernel(int kernelSize, double sigma); 
bool dataArrayShow(std::vector<double> array, cv::Mat src_show,cv::Scalar color = cv::Scalar(100,200,255));
