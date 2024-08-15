#include "blade.h"
#include "myFunction.h"
using namespace std;
using namespace cv;

void Blade::Init(vector<Point> contour, int contours_id)
{
    _contours_id = contours_id;           // 设置contours_id
    _contour = contour;                   // 设置contour
    _minAreaRect = minAreaRect(_contour); // 设置minAreaRect
    cv::Point2f points[4];
    _minAreaRect.points(points);
    _rrt_points.insert(_rrt_points.begin(), points, points + 4); // 设置_rrt_points;
    _center = _minAreaRect.center;                               // 设置center

    double len1, len2;
    len1 = getPointDistance(_rrt_points[0], _rrt_points[1]);
    len2 = getPointDistance(_rrt_points[1], _rrt_points[2]);
    myLine myline;
    if (len1 > len2)    //找到矩形中的长边，作为中垂线的斜率参照
        myline.init(_rrt_points[0], _rrt_points[1], _center);
    else
        myline.init(_rrt_points[1], _rrt_points[2], _center);
    _line = myline;
}

// 打印扇叶轮廓
void Blade::print_contour(cv::Mat src_img, Scalar color)
{
    drawPoints(src_img, _contour, color);
}
// 打印最小外接矩形
void Blade::print_minAreaRect(cv::Mat src_img, Scalar color)
{
    drawPoints(src_img, _rrt_points, color);
}

//打印扇叶的中垂线
void Blade::print_line(cv::Mat src_img,Scalar color)
{
    _line.print(src_img,color);
}