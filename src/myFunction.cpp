#include "myFunction.h"
#include "cmath"
using namespace cv;
using namespace std;

const cv::Scalar contours_scalar = cv::Scalar(200, 100, 0);
const int contours_thickness = 3;

Point2f getPointsCenter(vector<Point> points)
{
    Point2f center(0, 0);
    double P = 1.0 / points.size();
    for (auto &&point : points)
    {
        center += (Point2f)point * P;
    }

    return center;
}

double getPointDistance(Point p1, Point p2)
{
    return sqrt(pow((p1.x - p2.x), 2) + pow((p1.y - p2.y), 2));
}

double getPointDistance(Point2f p1, Point2f p2)
{
    return sqrt(pow((p1.x - p2.x), 2) + pow((p1.y - p2.y), 2));
}

void drawPoints(Mat src, vector<Point> points, Scalar color)
{
    int points_size = points.size();
    Point last_point = points[points_size - 1];

    for (auto &&point : points)
    {
        line(src, last_point, point, color, contours_thickness);
        last_point = point;
    }
}

void drawPoints(Mat src, vector<Point2f> points, Scalar color)
{
    int points_size = points.size();
    Point last_point = points[points_size - 1];

    for (auto &&point : points)
    {
        line(src, last_point, (Point)point, color, contours_thickness);
        last_point = point;
    }
}

void drawPoints(Mat src, Point2f points[], int len, Scalar color)
{
    vector<Point> vpoints(points, points + len);
    drawPoints(src, vpoints, color);
}

// 获取多个点的平均值
Point2f getPointsAverage(vector<Point2f> points)
{
    double P = 1.0 / points.size();
    Point2f AveragePoint(0, 0);
    for (auto &&point : points)
    {
        AveragePoint += point * P;
    }
    return AveragePoint;
}

// 求多个点的方差（以点的形式返回）
Point2f getPointsVar(vector<Point2f> points)
{
    double P = 1.0 / points.size();
    double P_sqrt = sqrt(P);
    Point2f p1(0, 0);
    Point2f p2(0, 0);
    for (auto &&point : points)
    {
        p1.x += P * point.x * point.x;
        p1.y += P * point.y * point.y;
        p2 += P * point;
    }
    p2 = Point2f(p2.x * p2.x, p2.y * p2.y);
    Point2f pointVar = p1 - p2;
    return pointVar;
}

// 用三个点求其共圆的圆心
Point2f getCircleCenter(Point2f p1, Point2f p2, Point2f p3)
{

    myLine L1, L2, L3, L4;
    L1.init(p1, p2);
    L2.init(p1, p3);
    double x1 = 0.5 * (p1.x + p2.x);
    double x2 = 0.5 * (p1.x + p3.x);
    L3 = L1.getPerpLine(x1);
    L4 = L2.getPerpLine(x2);

    Mat img = Mat::zeros(Size(1000, 1000), CV_8UC3);

    Point2f circleCenter = L3.intersectLine(L4);
    return circleCenter;
}

// 获取一维的高斯滤波核
vector<double> generateGaussianKernel(int kernelSize, double sigma)
{
    vector<double> kernel(kernelSize, 0.0);
    int mid = kernelSize / 2;
    double sum = 0.0;

    for (int i = 0; i < kernelSize; ++i)
    {
        int x = i - mid;
        kernel[i] = exp(-0.5 * x * x / (sigma * sigma)) / (sigma * sqrt(2 * M_PI));
        sum += kernel[i];
    }

    // 归一化
    for (auto &k : kernel)
    {
        k /= sum;
    }

    return kernel;
}

// 显示数据（线形图）
bool dataArrayShow(vector<double> array, Mat src_show,Scalar color)
{
    if(array.empty())return false;
    if(src_show.empty())return false;

    int spacing_x,spacing_y;
    double max_data = array[0];
    double min_data = array[0];
    for (auto &&_data : array)
    {
        if (_data > max_data)
            max_data = _data;
        if(_data < min_data)
            min_data = _data;
    }
    spacing_x = src_show.cols / array.size();
    spacing_y = src_show.rows / (max_data - min_data);
    if(spacing_y < 1)spacing_y = 1;
    if(spacing_x < 1)spacing_x = 1;

    vector<Point2f> points_show_array;

    int num = 1;
    for (auto &&data : array)
    {
        Point2f point(num * spacing_x, (data - min_data) * spacing_y);
        num++;
        points_show_array.push_back(point);
    }
    
    Point2f last_point = points_show_array[0];
    for(auto && point:points_show_array)
    {   
        line(src_show,last_point,point,color,3);
        last_point = point;
    }

    return true;
}