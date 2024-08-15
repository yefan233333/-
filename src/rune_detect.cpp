#include "rune_detect.h"
#include "myFunction.h"
#include "blade.h"
#include <iostream>
using namespace std;
using namespace cv;

int contour_min_area = 350;
int blade_min_area = 3000;
const double PI = 3.1415926;
const int blade_position_size = 30;
const int circleCenter_position_size = 30;

// 进行预处理、转化为二值化图像。
bool Rune_detect::img_Init(Mat src_img)
{
    if (src_img.empty())
        return false;

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
    _blade_position_matrix = Mat::zeros(_src_img.size(), CV_8UC1);        // 扇叶位置初始化
    _circleCenter_position_matrix = Mat::zeros(_src_img.size(), CV_8UC1); // 圆心位置初始化

    return true;
}

// 显示inti_img
bool Rune_detect::init_img_show(string winname)
{
    if (_init_img.empty())
        return false;
    imshow(winname, _init_img);
    return true;
}

// 显示src_img
bool Rune_detect::src_img_show(string winname)
{
    if (_src_img.empty())
        return false;
    imshow(winname, _src_img);
    return true;
}

// 获取src_img
Mat Rune_detect::get_src_img()
{
    return _src_img;
}

// 获取init_img
Mat Rune_detect::get_init_img()
{
    return _init_img;
}



// 检测二值化图像中的轮廓
bool Rune_detect::setBlades(Mat src_show)
{
    if (_init_img.empty())
        return false;

    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(_init_img, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE); // 获取到所有轮廓
    vector<Blade> bladeCapture;
    vector<Blade> blades;
    vector<Point2f> circle_center;

    int contours_id_array[100] = {0}; // 记录子轮廓的数量

    int hierarchy_id = 0;
    int contours_id = 0;

    int blade_num = BLADE_1; // 命名初始值
    int circleCenter_num = CIRCLECENTER_1;

    Mat _circleCenter_position_matrix_show = cv::Mat::zeros(_src_img.size(),CV_8UC3);
    for (auto &&contour : contours)
    {
        double C = arcLength(contour, true);
        double Area1 = PI * pow((C / 2 / PI), 2);
        double Area2 = contourArea(contour);
        double P = Area1 / Area2;

        if (Area2 < contour_min_area) // 过滤掉小面积噪音
        {
        }
        else if (0.8 < P && P < 1.5) // 发现圆形轮廓
        {
            Point2f center = getPointsCenter(contour);
            if (_circleCenter_position_matrix.at<uchar>(center) == 0) // 删除相互贴太近的点，防止在同一个地方重复提取。
            {
                circle_center.push_back(center);
                circle(_circleCenter_position_matrix, center, circleCenter_position_size, (circleCenter_num++ + 1)*(50), -1); // 在_circleCenter_position_matrix上标记这些点的位置。            
            }
        }
        else if (Area2 > blade_min_area) // 通过面积筛选得到扇叶轮廓
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
    // 将数据存入神符中。
    _blades = blades;
    _circle_centers = circle_center;


    if (!src_show.empty())
    {
        print_blades_contour(src_show);
        print_circle_center(src_show);
    }

    return true;
}

// 计算旋转中心（利用扇叶的中垂线以及三个圆心）
bool Rune_detect::setRotationCenter()
{
    std::vector<cv::Point2f> intersectPoints;
    Point2f rotationCenter_1, rotationCenter_2;
    for (int i = 1; i < _blades.size(); ++i)
    {
        for (int j = 1 + i; j < _blades.size(); ++j)
        {
            Point2f intersectPoint = _blades[i]._line.intersectLine(_blades[j]._line); // 获取两片扇叶中垂线之间的交点。
            intersectPoints.push_back(intersectPoint);
        }
    }
    rotationCenter_1 = getPointsAverage(intersectPoints);

    // 通过已有的圆心求旋转中心
    if (_circle_centers.size() >= 3)
    {
        rotationCenter_2 = getCircleCenter(_circle_centers[0], _circle_centers[1], _circle_centers[2]);
    }
    //rotationCenter_1：1000张图片时，误差为【2.6875，1.46875】，4000张图片时，误差为【1.15625，1.4375】
    //rotationCenter_2：1000张图片时，误差为【2.3125，2.00000】，4000张图片时，误差为【2.375，2.1875】
    //在图像帧少时，rotationCenter_2误差更小，图像帧多时，rotationCenter_1误差更小。
    //结合两个所得量，根据误差进行加权求和，得到最终结果_rotationCenter：1000张图片时，误差为【1.178125，0.8125】，4000张图片时，误差为【3.40625，0.9375】

    if(rotationCenter_2.x != 0)
        _rotationCenter = 2.3125/(2.6875 + 2.3125) * rotationCenter_1 + 2.6875/(2.6875 + 2.3125) * rotationCenter_2;
    else 
        _rotationCenter = rotationCenter_1;

    if (intersectPoints.size() == 0)
        return false;
    else
        return true;
}

// 打印五片扇叶
bool Rune_detect::print_blades_contour(Mat src_show)
{
    if (src_show.empty())
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
    if (src_show.empty())
        return false;

    _blades[0].print_minAreaRect(src_show, Scalar(0, 100, 200));

    for (int i = 1; i < _blades.size(); i++)
    {
        _blades[i].print_minAreaRect(src_show, Scalar(200, 100, 0));
    }

    return true;
}

// 打印扇叶的中垂线
bool Rune_detect::print_blades_line(Mat src_show)
{
    for (auto &&blade : _blades)
    {
        blade.print_line(src_show, Scalar(100, 0, 200));
    }
}

// 打印神符中的圆心（打击点）
bool Rune_detect::print_circle_center(Mat src_show)
{
    if (src_show.empty())
    {
        return false;
    }
    for (auto &&center : _circle_centers)
    {
        circle(src_show, center, 6, Scalar(200, 100, 200), -1);
    }
    return true;
}

bool Rune_detect::print_rotationCenter(Mat src_show)
{
    if (src_show.empty())
    {
        return false;
    }
    circle(src_show, _rotationCenter, 3, Scalar(200, 60, 0), -1);
    return true;
}