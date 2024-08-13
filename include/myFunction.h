#include<iostream>
#include<opencv2/opencv.hpp>
#include<vector>

// int contour_min_area = 350;
extern const cv::Scalar contours_scalar;
extern const int contours_thickness;

cv::Point2f getPointsCenter(std::vector<cv::Point> points);


void drawPoints(cv::Mat src,std::vector<cv::Point> points,cv::Scalar color);


void drawPoints(cv::Mat src,cv::Point2f points[],int len,cv::Scalar color);
