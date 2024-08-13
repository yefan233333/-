#include"myFunction.h"
using namespace cv;
using namespace std;


Point2f getPointsCenter(vector<Point> points)
{
    Point2f center(0,0);
    double P = 1.0 / points.size();
    for(auto&& point:points)
    {
        center += (Point2f)point * P;
    }

    return center;
}

void drawPoints(Mat src,vector<Point> points)
{
    int points_size = points.size();
    Point last_point = points[points_size -1];
    for(auto&& point:points)
    {
        line(src,last_point,point,contours_scalar,contours_thickness);
        last_point = point;
    }
}

void drawPoints(Mat src,Point2f points[],int len)
{
    vector<Point> vpoints(points,points + len);
    drawPoints(src,vpoints);
}