#pragma once
#include<iostream>
#include<vector>
#include<opencv2/opencv.hpp>
#include"blade.h"
#include<string>
#include <utility>



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
    cv::Mat _blade_position_matrix;          //用于存储当前帧的每一片扇叶的位置。方便连续帧之间的扇叶跟踪。
    cv::Mat _circleCenter_position_matrix;   //用于存储当前帧的每一个圆心的位置。方便连续帧之间的圆心跟踪。
public:
    cv::Point2f _rotationCenter;
    bool img_Init(cv::Mat src_img);
    bool init_img_show(std::string winname = "init_img");
    bool src_img_show(std::string winname = "src_img");
    cv::Mat get_src_img();
    cv::Mat get_init_img();
    int get_Blade_size();
    int get_CircleCenter_size();
    cv::Mat get_blade_position_matrix();
    cv::Mat get_circleCenter_position_matrix();
    std::vector<Blade> get_blades();
    std::vector<cv::Point2f> get_circle_centers();
    bool setBlades(cv::Mat src_show = cv::Mat());   
    bool setRotationCenter();
    bool print_blades_minAreaRect(cv::Mat src_show);
    bool print_blades_contour(cv::Mat src_show);      
    bool print_blades_line(cv::Mat src_show);
    bool print_circle_center(cv::Mat src_show);
    bool print_rotationCenter(cv::Mat src_show);

};

enum blade_id
{
    BLADE_1,
    BLADE_2,
    BLADE_3,
    BLADE_4,
    BLADE_5
};

enum circleCenter_id
{
    CIRCLECENTER_1,
    CIRCLECENTER_2,
    CIRCLECENTER_3,
    CIRCLECENTER_4,
    CIRCLECENTER_5
};