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
    Mat img_show;
    string show_winname = "img show";
    namedWindow("src_show",WINDOW_AUTOSIZE);
    createTrackbar("contour_min_area",show_winname,&contour_min_area,2000);
    createTrackbar("blade_min_area",show_winname,&blade_min_area,4000);
    
    while(1)
    {
        vid.read(img);
        if(img.empty())
            break;
        imshow("img",img);
        Rune_detect rune_detect;
        rune_detect.img_Init(img);
        rune_detect.getBlades();
        img_show = rune_detect.get_src_img();
        rune_detect.print_blades_minAreaRect(img_show);
        rune_detect.print_circle_center(img_show);
        imshow(show_winname,img_show);
    

        if(waitKey(10) == 'q')break;
    }
    waitKey(0);
    return 0;
}