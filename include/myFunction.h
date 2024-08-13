#include<iostream>
#include<opencv2/opencv.hpp>
#include<vector>

// int contour_min_area = 350;
const cv::Scalar contours_scalar = cv::Scalar(200,100,0);
const int contours_thickness = 3;

cv::Point2f getPointsCenter(std::vector<cv::Point> points);


void drawPoints(cv::Mat src,std::vector<cv::Point> points);


void drawPoints(cv::Mat src,cv::Point2f points[],int len);
