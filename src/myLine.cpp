#include"myLine.h"
#include"printf.h"
using namespace std;
using namespace cv;

//通过 y = k*x + m 构造直线方程
myLine::myLine(double k,double m)   
{
    _A = k;
    _B = -1;
    _C = m;
}

//构造一条过p3、平行于p1和p2连线的直线。表达式为：Ax + By + C = 0
myLine::myLine(cv::Point2f p1,cv::Point2f p2,cv::Point2f p3)    
{
    this->init(p1,p2,p3);
}

//通过两点构造一条直线
myLine::myLine(cv::Point2f p1,cv::Point2f p2)
{
    this->init(p1,p2);
}

//构造一条过p3、平行于p1和p2连线的直线。表达式为：Ax + By + C = 0
void myLine::init(cv::Point2f p1,cv::Point2f p2,cv::Point2f p3)
{
    Point2f v1 = p2 - p1;
    _A = -v1.y;
    _B = v1.x;
    _C = -(_A * p3.x + _B * p3.y);
}

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
        cout << "det = "<<det <<endl;
        return Point2f(0,0);
    }

    double x = (-1*_C * l2._B - _B * -1*l2._C) / det;  // 解出 x
    double y = (_A * -1*l2._C - -1*_C * l2._A) / det;  // 解出 y

    return Point2f(x, y);  // 返回交点坐标
}

//过两个点构造直线
void myLine::init(Point2f p1,Point2f p2)
{
    Point2f v1 = p2 - p1;
    _A = -v1.y;
    _B = v1.x;
    _C = -(_A * p2.x + _B * p2.y);
    // _C = -(_A * p2.x + _B * p2.y);
}

//获取该直线的垂线。x为交点横坐标
myLine myLine::getPerpLine(double x)
{
    double A = -_B;
    double B = _A;
    double C = -(A*x + B*get_y(x));
    myLine result(A,B,C);
    return result;
}

ostream& operator<<(ostream& cout,const myLine& myline)
{
    cout << "["<<myline._A<<","<<myline._B<<","<<myline._C<<"]" <<endl;
    return cout;
}


