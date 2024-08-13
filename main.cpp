#include<iostream>
#include <opencv2/opencv.hpp>
#include<vector>
#include<cmath>
#include"rune_detect.h"
#include<string>
#include"myFunction.h"

using namespace std;
using namespace cv;



void contour_min_area_back(int,void*)
{
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

    
    while(1)
    {
        vid.read(img);
        if(img.empty())
            break;
        imshow("img",img);
        Rune_delete rune_delete;

        img_init = rune_delete.img_Init1(img);
        imshow("img_init",img_init);
        img_contours_show = Mat::zeros(img_init.size(),img.type());
        rune_delete.getContours(img_init,img_contours_show);
        if(waitKey(10) == 'q')break;
    }
    waitKey(0);
    return 0;
}