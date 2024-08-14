#pragma once
#include<iostream>
#include<opencv2/opencv.hpp>
#include<vector>
#include<cmath>

class myLine
{
public:
    double _A;
    double _B;
    double _C;
    myLine(){};
    myLine(double A,double B,double C):_A(A),_B(B),_C(C){};
    myLine(double k,double m)   //通过 y = k*x + m 构造直线方程
    {
        _A = k;
        _B = -1;
        _C = m;
    }
    myLine(cv::Point2f p1,cv::Point2f p2,cv::Point2f p3)    //构造一条过p3、平行于p1和p2连线的直线。表达式为：Ax + By + C = 0
    {
        cv::Point2f v1 = p2 - p1;
        _A = -v1.y;
        _B = v1.x;
        _C = -(_A * p3.x + _B * p3.y);
    }
    double get_x(double y);
    double get_y(double x);
    void print(cv::Mat src_show,cv::Scalar color = cv::Scalar(200,40,140));
    cv::Point2f intersectLine(myLine l2);

};