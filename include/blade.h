#pragma once
#include<iostream>
#include<opencv2/opencv.hpp>
#include<vector>

//扇叶
class Blade
{
private:
    std::vector<cv::Point> _contour;    //扇叶的轮廓
    cv::RotatedRect _rotatedRect;       //扇叶的最小面积矩形
    cv::Point2f _rrt_points[4];         //最小面积矩形的四个点
    cv::Point2f _center;                //扇叶的中心
public:
    Blade(){};
    Blade(std::vector<cv::Point> contour)
    {
        _contour = contour;
        _rotatedRect = minAreaRect(_contour);
        _rotatedRect.points(_rrt_points);
        _center = _rotatedRect.center;
    }
    

};