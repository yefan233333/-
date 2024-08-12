#include<iostream>
#include <opencv2/opencv.hpp>
#include<vector>
#include<cmath>
#include"rune_detect.h"
using namespace std;
using namespace cv;

//整体思路：

const double PI = 3.1415926;
int contour_min_area = 1000;

void contour_min_area_back(int,void*)
{
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
Mat getContours(Mat src)
{
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(src,contours,hierarchy,RETR_TREE,CHAIN_APPROX_SIMPLE); //获取到所有轮廓
    Mat contours_show = Mat::zeros(src.size(),CV_8UC3);
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
        if(0.7 < P && P <1.3)   //如果该轮廓是圆形，就把这个轮廓转移到circle中。并舍弃该轮廓
        {
            circleCenters.push_back(contour);
            cout << "Area1 = " <<Area1 <<" ";
            cout << "Area2 = " <<Area2 <<endl;
            cout << "P = " << P <<endl;
            continue;   
        }
        good_contours.push_back(contour);
    }
    drawContours(contours_show,good_contours,-1,Scalar(200,100,0),1);
    imshow("contours_show",contours_show);

    //进行最小面积矩形拟合

    return contours_show;
}


int main()
{
    VideoCapture vid;
    vid.open("/home/yefan/桌面/神符检验.avi");
    Mat img;
    Mat dst;
    namedWindow("contours_show",WINDOW_AUTOSIZE);
    createTrackbar("contour_min_area","contours_show",&contour_min_area,2000);
    rune_delete rd(70);
    rd.show();

    
    while(1)
    {
        vid.read(img);
        if(img.empty())
            break;
        imshow("img",img);
        dst = img_Init1(img);
        imshow("dst",dst);
        Mat contours_show = getContours(dst);
        contours_show += img;
        // imshow("contours_show",contours_show);
        if(waitKey(10) == 'q')break;
    }

    cout << "视频播放完毕"<<endl;

    waitKey(0);
    return 0;
}