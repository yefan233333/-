#pragma once
#include<iostream>
#include<opencv2/opencv.hpp>
#include<vector>
#include"myFunction.h"
#include"myLine.h"

//扇叶
class Blade
{
private:
    
    std::vector<cv::Point> _contour;    //扇叶的轮廓
    cv::RotatedRect _minAreaRect;       //扇叶的最小面积矩形
    std::vector<cv::Point2f> _rrt_points;         //最小面积矩形的四个点
public:
    int _contours_id;    //该扇叶轮廓在contours中的下标
    myLine _line;   //扇叶的中垂线（指向旋转中心）
    cv::Point2f _center;                //扇叶的中心
    Blade(){};
    Blade(std::vector<cv::Point> contour,int contours_id);
    void Init(std::vector<cv::Point> contour,int contours_id);
    void print_contour(cv::Mat src_img,cv::Scalar color = contours_scalar); //显示扇叶轮廓
    void print_minAreaRect(cv::Mat src_img,cv::Scalar color = contours_scalar);
    void print_line(cv::Mat src_img,cv::Scalar color = contours_scalar);

};

