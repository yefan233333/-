#pragma once
#include<iostream>
#include<opencv2/opencv.hpp>
#include<vector>
#include<cmath>
#include"rune_detect.h"
#include"blade.h"
#include"myFunction.h"
#include<string>
#include <memory>
#include<deque>

//用于处理多张连续的神符图像
class runeVideoProcessor
{
private:
    std::unique_ptr<Rune_detect> _rune_detect_ptr;   //记录最新帧的神符状态
    std::unique_ptr<Rune_detect> _rune_detect_last_ptr;  //记录上一帧的神符状态
    std::vector<Blade> _blades; //用于保存五个神符的最新信息
    std::vector<std::vector<cv::Point2f>> _blades_position_array;   //保存五个神符的历史信息 时间 -> 五片扇叶中心
    cv::Point2f _rotationCenter;     //记录神符的旋转中心
    double _rotationSpeed;      //记录神符的当前旋转速度
    std::vector<double> _rotationSpeed_array; //记录神符的历史旋转速度
    std::deque<std::vector<cv::Point2f>> _blades_polar_position_array; //存储一段时间内的扇叶中心的极坐标数据，方便计算速度。

    void Init();
    void setRotationCenter();
    void setBladePosition();   
    void setSpeed();
    bool _start_flag;
    int _num;   //记录图像读取的次数
public: 
    cv::Mat _src;       //存储最新读到的图片
    runeVideoProcessor();
    bool read(cv::Mat src);    //读取新的图片、进行位置更新
    bool src_show(std::string winname = "src");   //展示读取到的新图片
    bool show(std::string winname = "runeVideoProcessor show");   //展示识别效果
    bool getStartFlag();
    int getNum();
    bool print_rotationCenter(cv::Mat src_show);
    bool print_blades_angle(cv::Mat src_show);
    bool print_blades_num(cv::Mat src_show);


};


