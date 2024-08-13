#include"rune_detect.h"
#include"myFunction.h"
#include"blade.h"
using namespace std;
using namespace cv;


int contour_min_area = 350;
int blade_min_area = 3000;
const double PI = 3.1415926;

//进行预处理、转化为二值化图像。
Mat Rune_delete::img_Init1(Mat src)
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
void Rune_delete::getBlades(Mat src,Mat src_show)
{
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(src,contours,hierarchy,RETR_TREE,CHAIN_APPROX_SIMPLE); //获取到所有轮廓
    vector<vector<Point>> circleCenters;
    vector<Blade> blades;
    int contours_id_array[100] = {0};   //记录子轮廓的数量

    int hierarchy_id = 0;
    int contours_id = 0;
    for(auto&& contour:contours)
    {
        double C = arcLength(contour,true);
        double Area1 = PI * pow((C/2/PI),2);
        double Area2 = contourArea(contour);
        double P = Area1/Area2;
        if(Area2 < contour_min_area)
        {

        }
        else if(0.6 < P && P <1.5)   //如果该轮廓是圆形，就把这个轮廓转移到circle中。并舍弃该轮廓
        {
            circleCenters.push_back(contour);
            
            Point2f center = getPointsCenter(contour);
            //显示圆心的位置
            circle(src_show,center,4,Scalar(50,200,50),-1); 
        }
        else if(Area2 > blade_min_area)
        {
            //通过面积筛选得到扇叶轮廓

            Blade blade(contour,contours_id);
            // blade.contour_show(img_getContours_show);   //用于显示扇叶的最小外接矩形
            blades.push_back(blade);

        }
        else
        {
            //既不是扇叶、又不是打击圆的轮廓，其父轮廓的子轮廓数+1
            contours_id_array[hierarchy[hierarchy_id][3]]++;
            // Blade blade(contour,contours_id);
            // blade.contour_show(src_show);
            // int father_contour_id = hierarchy[hierarchy_id][3];
            // putText(src_show,to_string(father_contour_id),blade._center,FONT_HERSHEY_SIMPLEX,0.5,Scalar(200,100,0),0.5);
        }

        hierarchy_id++;
        contours_id++;
    }

    _blades.resize(1);
    for(auto&& blade:blades)
    {
        if(contours_id_array[blade._contours_id] == 2)  //当子轮廓数为2时，则该扇叶为已打击扇叶。
        {
            _blades.push_back(blade);
            blade.contour_show(src_show,Scalar(200,100,0));

        }
        else
        {
            _blades[0] = blade;
            blade.contour_show(src_show,Scalar(0,100,200));

        }
        cout << contours_id_array[blade._contours_id] <<" ";
    }
    cout << "blades.size = " <<blades.size() <<endl;
    drawContours(src_show,contours,-1,Scalar(200,100,0),1);
    // cout << "contours.size = " <<contours.size() <<endl;

    // drawContours(img_getContours_show,good_contours,-1,Scalar(200,100,0),1);
    imshow("src_show",src_show);
    //进行最小面积矩形拟合
}