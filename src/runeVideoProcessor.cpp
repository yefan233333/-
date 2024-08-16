#include"runeVideoProcessor.h"
using namespace std;
using namespace cv;


//初始化(读取到第一帧合适画面时开始工作)
void runeVideoProcessor::Init()
{
    _blades = _rune_detect_last_ptr->get_blades();

}

//读取新的图片、进行位置更新
bool runeVideoProcessor::imread(cv::Mat src)  
{
    if(src.empty())return false;
    
    //获取当前帧扇叶的计算
    _rune_detect_ptr.reset(new Rune_detect);
    _rune_detect_ptr->img_Init(src);
    _rune_detect_ptr->setBlades();
    _rune_detect_ptr->setRotationCenter();

    static double num = 0;     //记录读取图像的次数
    num++;

    static bool Init_flag = false;
    if(!Init_flag)  
    {
        if(_rune_detect_ptr->get_CircleCenter_size() == 3)  //当读取到的图像帧能识别到3个圆心时进行初始化。
        {
            this->Init();
        }
    }
    else 
    {
        
    }

    _rune_detect_last_ptr = move(_rune_detect_ptr);     //把_rune_detect_ptr传递给_rune_detect_last_ptr；
}  


//展示读取到的新图片
bool runeVideoProcessor::src_imshow(std::string winname)
{

}


//展示识别效果
bool runeVideoProcessor::imshow(std::string winname)
{

}