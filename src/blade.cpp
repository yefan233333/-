#include"blade.h"
#include"myFunction.h"
using namespace std;
using namespace cv;

void Blade::contour_show(cv::Mat img_show,Scalar color)
{
    vector<Point> points(_rrt_points,_rrt_points+4);
    drawPoints(img_show,points,color);
}