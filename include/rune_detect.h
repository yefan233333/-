#pragma once
#include<iostream>
#include<vector>
#include<opencv2/opencv.hpp>


extern int contour_min_area;
extern const double PI;

class Rune_delete
{
private:
    cv::Mat img;    //操作的图像

public:
    cv::Mat img_Init1(cv::Mat src);
    void getContours(cv::Mat src,cv::Mat src_show);



};