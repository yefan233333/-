#include<iostream>
#include <opencv2/opencv.hpp>
#include<vector>
#include<cmath>
#include"rune_detect.h"
#include<string>
using namespace std;
using namespace cv;

//整体思路：

const double PI = 3.1415926;
int contour_min_area = 400;
const Scalar contours_scalar = Scalar(200,100,0);
const int contours_thickness = 3;

void contour_min_area_back(int,void*)
{
}

Point2f getPointsCenter(vector<Point> points)
{
    double center_x = 0;
    double center_y = 0;
    double P = 1.0 / points.size();
    for(auto&& point:points)
    {
        center_x += point.x * P;
        center_y += point.y * P;
    }
    Point2f center(center_x,center_y);
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



//进行预处理、转化为二值化图像。
Mat img_Init1(Mat src)
{   
    Mat src_split[3];
    split(src,src_split);
    Mat src_red;
    src_red = 0.5 * (src_split[2] - src_split[0])+ 0.5 * (src_split[2] - src_split[1]);
    Mat src_inrange;
    inRange(src_red,30,255,src_inrange);//进行二值化处理
    Mat src_open;
    Mat kernel_open = getStructuringElement(MORPH_RECT,Size(2,2));
    morphologyEx(src_inrange,src_open,MORPH_CLOSE,kernel_open);
    return src_inrange;
}

//检测二值化图像中的轮廓
void getContours(Mat src,Mat src_show)
{
    Mat img_getContours_show = src_show.clone();
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(src,contours,hierarchy,RETR_TREE,CHAIN_APPROX_SIMPLE); //获取到所有轮廓
    vector<vector<Point>> circleCenters;
    vector<vector<Point>> good_contours;
    for(auto& contour:contours)
    {
        double C = arcLength(contour,true);
        double Area1 = PI * pow((C/2/PI),2);
        double Area2 = contourArea(contour);
        double P = Area1/Area2;
        if(Area2 < contour_min_area)
        {
            continue;
        }
        else if(0.6 < P && P <1.5)   //如果该轮廓是圆形，就把这个轮廓转移到circle中。并舍弃该轮廓
        {
            circleCenters.push_back(contour);
            
            Point2f center = getPointsCenter(contour);
            //显示圆心的位置
            circle(img_getContours_show,center,4,Scalar(50,200,50),-1);
            continue;      
        }
        else 
        {
            //判断合格的轮廓，进行最小矩形拟合。
            RotatedRect rrt = minAreaRect(contour);
            Point2f points[4];
            rrt.points(points);
            drawPoints(img_getContours_show,points,4);

            //在矩形的中心位置标记P值
            // putText(img_getContours_show,to_string(P),rrt.center,FONT_HERSHEY_SIMPLEX,0.3,Scalar(0,100,200),0.3);

            good_contours.push_back(contour);
        }
    }

    drawContours(img_getContours_show,good_contours,-1,Scalar(200,100,0),1);
    imshow("img_getContours_show",img_getContours_show);
    //进行最小面积矩形拟合
}


int main()
{
    VideoCapture vid;
    vid.open("/home/yefan/桌面/神符检验.avi");
    Mat img;
    Mat img_init;
    Mat img_contours_show;
    namedWindow("img_getContours_show",WINDOW_AUTOSIZE);
    createTrackbar("contour_min_area","img_getContours_show",&contour_min_area,2000);
    rune_delete rd(70);
    rd.show();

    
    while(1)
    {
        vid.read(img);
        if(img.empty())
            break;
        imshow("img",img);
        img_init = img_Init1(img);
        imshow("img_init",img_init);
        img_contours_show = Mat::zeros(img_init.size(),img.type());
        getContours(img_init,img_contours_show);
        if(waitKey(10) == 'q')break;
    }
    waitKey(0);
    return 0;
}