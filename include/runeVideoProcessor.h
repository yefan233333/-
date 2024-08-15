#pragma once
#include<iostream>
#include<opencv2/opencv.hpp>
#include<vector>
#include<cmath>
#include"rune_detect.h"
#include"blade.h"
#include"myFunction.h"
#include<string>

//用于处理多张连续的神符图像
class runeVideoProcessor
{
private:
    Rune_detect _rune_detect;   //记录最新帧的神符状态
    Rune_detect _rune_detect_last;  //记录上一帧的神符状态
    std::vector<Blade> _blades; //用于保存五个神符的信息
    double _rotationCenter;     //记录神符的旋转中心
    double _rotationSpeed;      //记录神符的当前旋转速度
    double _rotationSpeed_array; //记录神符的历史旋转速度
public: 
    bool imread(cv::Mat src);    //读取新的图片、进行位置更新
    bool src_imshow(std::string winname);   //展示读取到的新图片
    bool imshow(std::string winname);   //展示识别效果
};


