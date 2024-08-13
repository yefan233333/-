#pragma once
#include<iostream>
#include<vector>
#include<opencv2/opencv.hpp>
#include"blade.h"


extern int contour_min_area;
extern int blade_min_area;
extern const double PI;

//该类用于存储一帧图像中的整个神符的各项元素。
class Rune_delete
{
private:
    cv::Mat _img;    //操作的图像
    std::vector<Blade> _blades;  //用于储存五片扇叶，其第一个元素为未击打扇叶、其余均为已击打扇叶。

public:
    Rune_delete(cv::Mat img)
    {
        _img = img;
    }
    cv::Mat img_Init1(cv::Mat src);
    void getBlades(cv::Mat src,cv::Mat src_show);



};