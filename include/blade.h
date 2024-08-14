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
    std::vector<cv::Point2f> _rrt_points;         //最小面积矩形的四个点
public:
    int _contours_id;    //该扇叶轮廓在contours中的下标
    cv::Point2f _center;                //扇叶的中心
    Blade(){};
    Blade(std::vector<cv::Point> contour,int contours_id)
    {
        _contours_id = contours_id;
        _contour = contour;
        _rotatedRect = minAreaRect(_contour);
        cv::Point2f points[4];
        _rotatedRect.points(points);
        _rrt_points.insert(_rrt_points.begin(),points,points+4);

        _center = _rotatedRect.center;
    }
    void print_contour(cv::Mat src_img,cv::Scalar color = contours_scalar); //显示扇叶轮廓
    void print_minAreaRect(cv::Mat src_img,cv::Scalar color = contours_scalar);
    

};

