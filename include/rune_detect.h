#pragma once
#include<iostream>
#include<vector>
#include<opencv2/opencv.hpp>
#include"blade.h"
#include<string>


extern int contour_min_area;
extern int blade_min_area;
extern const double PI;

//该类用于存储一帧图像中的整个神符的各项元素。
class Rune_detect
{
private:
    cv::Mat _src_img;    //操作的图像
    cv::Mat _init_img;  //初始化后的图像
    std::vector<Blade> _blades;  //用于储存五片扇叶，其第一个元素为未击打扇叶、其余均为已击打扇叶。
    std::vector<cv::Point2f> _circle_centers;   //用于存储识别到的圆形圆心
public:

    bool img_Init(cv::Mat src_img);
    bool init_img_show(std::string winname = "init_img");
    bool src_img_show(std::string winname = "src_img");
    cv::Mat get_src_img();
    cv::Mat get_init_img();
    bool getBlades(cv::Mat src_show = cv::Mat());   
    bool print_blades_minAreaRect(cv::Mat src_show);
    bool print_blades_contour(cv::Mat src_show);      
    bool print_circle_center(cv::Mat src_show);
    


};