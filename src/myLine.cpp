#include"myLine.h"
using namespace std;
using namespace cv;


double myLine::get_x(double y)
{
    return (-_B*y - _C)/_A;
}

double myLine::get_y(double x)
{
    return (-_A*x - _C)/_B;
}

//在图像上绘制该直线
void myLine::print(Mat src_show,Scalar color)
{
    double x1,x2,y1,y2;
    x1 = 0;
    x2 = src_show.cols;
    y1 = this->get_y(x1);
    y2 = this->get_y(x2);
    line(src_show,Point(x1,y1),Point(x2,y2),color,2);

}

//返回两条直线的交点。
Point2f myLine::intersectLine(myLine l2)
{
    //使用克莱姆法则求解交点
    double det = _A * l2._B - _B * l2._A;  // 计算行列式的值，即系数矩阵的行列式

    if (det == 0)
    {
        // 如果行列式为0，则直线平行或者重合
        cout <<"直线发生e重合或者平行,提示错误"<<endl;
        return Point2f(0,0);
    }

    double x = (-1*_C * l2._B - _B * -1*l2._C) / det;  // 解出 x
    double y = (_A * -1*l2._C - -1*_C * l2._A) / det;  // 解出 y

    return Point2f(x, y);  // 返回交点坐标
}