#include "rune_detect.h"
#include "myFunction.h"
#include "blade.h"
using namespace std;
using namespace cv;

int contour_min_area = 350;
int blade_min_area = 3000;
const double PI = 3.1415926;

// 进行预处理、转化为二值化图像。
bool Rune_detect::img_Init(Mat src_img)
{
    if(src_img.empty())return false;

    _src_img = src_img;
    Mat src_split[3];
    split(src_img, src_split);
    Mat src_red;
    src_red = 0.5 * (src_split[2] - src_split[0]) + 0.5 * (src_split[2] - src_split[1]);
    Mat src_inrange;
    inRange(src_red, 30, 255, src_inrange); // 进行二值化处理
    Mat src_open;
    Mat kernel_open = getStructuringElement(MORPH_RECT, Size(2, 2));
    morphologyEx(src_inrange, src_open, MORPH_CLOSE, kernel_open);

    _init_img = src_inrange.clone();
    return true;
}

//显示inti_img
bool Rune_detect::init_img_show(string winname)
{
    if(_init_img.empty())return false;
    imshow(winname,_init_img);
    return true;
}

//显示src_img
bool Rune_detect::src_img_show(string winname)
{
    if(_src_img.empty())return false;
    imshow(winname,_src_img);
    return true;
}

//获取src_img
Mat Rune_detect::get_src_img()
{
    return _src_img;
}

//获取init_img
Mat Rune_detect::get_init_img()
{
    return _init_img;
}

// 检测二值化图像中的轮廓
bool Rune_detect::getBlades(Mat src_show)
{
    if(_init_img.empty())return false;

    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(_init_img, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE); // 获取到所有轮廓
    vector<vector<Point>> circleCenters;
    vector<Blade> bladeCapture;
    vector<Blade> blades;
    vector<Point2f> circle_center;
    
    int contours_id_array[100] = {0}; // 记录子轮廓的数量

    int hierarchy_id = 0;
    int contours_id = 0;
    for (auto &&contour : contours)
    {
        double C = arcLength(contour, true);
        double Area1 = PI * pow((C / 2 / PI), 2);
        double Area2 = contourArea(contour);
        double P = Area1 / Area2;
        if (Area2 < contour_min_area)
        {
        }
        else if (0.6 < P && P < 1.5) // 如果该轮廓是圆形，就把这个轮廓转移到circle中。并舍弃该轮廓
        {
            circleCenters.push_back(contour);
            Point2f center = getPointsCenter(contour);
            circle_center.push_back(center);
        }
        else if (Area2 > blade_min_area)// 通过面积筛选得到扇叶轮廓
        {
            Blade blade(contour, contours_id);
            bladeCapture.push_back(blade);
        }
        else
        {
            contours_id_array[hierarchy[hierarchy_id][3]]++;
        }

        hierarchy_id++;
        contours_id++;
    }

    blades.resize(1);
    for (auto &&blade : bladeCapture)
    {
        if (contours_id_array[blade._contours_id] == 2) // 已打击扇叶。
        {
            blades.push_back(blade);
        }
        else // 未击打扇叶
        {
            blades[0] = blade;
        }
    }

    //将数据存入神符中。
    _blades = blades;
    _circle_centers = circle_center;

    if(!src_show.empty())
    {
        print_blades_contour(src_show);
        print_circle_center(src_show);
    }

    return true;
}

//打印五片扇叶
bool Rune_detect::print_blades_contour(Mat src_show)
{
    if(src_show.empty())
        return false;

    _blades[0].print_contour(src_show, Scalar(0, 100, 200));
    
    for (int i = 1; i < _blades.size(); i++)
    {
        _blades[i].print_contour(src_show, Scalar(200, 100, 0));
    }

    return true;
}

bool Rune_detect::print_blades_minAreaRect(Mat src_show)
{
    if(src_show.empty())
        return false;

    _blades[0].print_minAreaRect(src_show, Scalar(0, 100, 200));
    
    for (int i = 1; i < _blades.size(); i++)
    {
        _blades[i].print_minAreaRect(src_show, Scalar(200, 100, 0));
    }

    return true;
}

//打印神符中的圆心（打击点）
bool Rune_detect::print_circle_center(cv::Mat src_show)
{
    if(src_show.empty())
    {
        return false;
    }
    for(auto&& center:_circle_centers)
    {
        circle(src_show,center,6,Scalar(200,100,200),-1);
    }
    return true;
}
