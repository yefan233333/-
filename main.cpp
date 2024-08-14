#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <cmath>
#include "rune_detect.h"
#include <string>
#include "myFunction.h"
#include "myLine.h"

using namespace std;
using namespace cv;

void contour_min_area_back(int, void *)
{
}

void test1()
{
    VideoCapture vid;
    vid.open("/home/yefan/桌面/神符检验.avi");
    Mat img;
    Mat img_init;
    Mat img_show;
    string show_winname = "img show";
    namedWindow(show_winname, WINDOW_AUTOSIZE);
    createTrackbar("contour_min_area", show_winname, &contour_min_area, 2000);
    createTrackbar("blade_min_area", show_winname, &blade_min_area, 4000);

    while (1)
    {
        vid.read(img);
        if (img.empty())
            break;
        imshow("img", img);
        Rune_detect rune_detect;
        rune_detect.img_Init(img);
        rune_detect.getBlades();
        img_show = rune_detect.get_src_img();
        rune_detect.print_blades_minAreaRect(img_show);
        rune_detect.print_circle_center(img_show);
        imshow(show_winname, img_show);

        if (waitKey(10) == 'q')
            break;
    }
    waitKey(0);
}

void myLine_test()
{

    VideoCapture vid(0);
    string show_winname = "img show";
    int k1 = 0.5;
    int k2 = 0.5;
    int m1 = 500;
    int m2 = 500;
    namedWindow(show_winname,WINDOW_AUTOSIZE);
    createTrackbar("k1", show_winname, &k1, 200);
    createTrackbar("m1", show_winname, &m1, 1000);
    createTrackbar("k2", show_winname, &k2, 200);
    createTrackbar("m2", show_winname, &m2, 1000);

    while (1)
    {
        Mat img = Mat::zeros(Size(1000, 500), CV_8UC3);
        myLine l1(k1*0.01 - 0.5,m1 - 500);
        l1.print(img);
        myLine l2(k2*0.01 - 0.5,m2 - 500);
        l2.print(img,Scalar(0,100,200));
        circle(img, l1.intersectLine(l2), 10, Scalar(200, 100, 0), -1);

        imshow(show_winname, img);
        waitKey(1);
    }

    waitKey(0);
}

int main()
{
    // test1();
    myLine_test();
    return 0;
}