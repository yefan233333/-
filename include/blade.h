#pragma once
#include<iostream>
#include<opencv2/opencv.hpp>
#include<vector>
#include"myFunction.h"

//扇叶
class Blade
{
private:
    
    std::vector<cv::Point> _contour;    //扇叶的轮廓
    cv::RotatedRect _rotatedRect;       //扇叶的最小面积矩形
    cv::Point2f _rrt_points[4];         //最小面积矩形的四个点
public:
    int _contours_id;    //该扇叶轮廓在contours中的下标
    cv::Point2f _center;                //扇叶的中心
    Blade(){};
    Blade(std::vector<cv::Point> contour,int contours_id)
    {
        _contours_id = contours_id;
        _contour = contour;
        _rotatedRect = minAreaRect(_contour);
        _rotatedRect.points(_rrt_points);
        _center = _rotatedRect.center;
    }
    void contour_show(cv::Mat img_show,cv::Scalar color = contours_scalar);

    

};

