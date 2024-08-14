#include"blade.h"
#include"myFunction.h"
using namespace std;
using namespace cv;

void Blade::print_contour(cv::Mat src_img,Scalar color)
{
    drawPoints(src_img,_contour,color);
}

void Blade::print_minAreaRect(cv::Mat src_img,Scalar color)
{
    drawPoints(src_img,_rrt_points,color);
}