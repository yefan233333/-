#include "runeVideoProcessor.h"
using namespace std;
using namespace cv;


runeVideoProcessor::runeVideoProcessor()
{
    _start_flag = false;
    _num = 0;
}

// 初始化(读取到第一帧合适画面时开始工作)
void runeVideoProcessor::Init()
{
    _blades = _rune_detect_ptr->get_blades();

    vector<Point2f> blades_position;
    for (auto &&blade : _blades)
    {
        blades_position.push_back(blade._center);
    }
    _blades_position_array.push_back(blades_position);
}

// 读取新的图片、进行位置更新
bool runeVideoProcessor::read(cv::Mat src)
{
    if (src.empty())
        return false;


    _num ++;
    _src = src;
    // 获取当前帧扇叶的计算
    _rune_detect_ptr.reset(new Rune_detect);
    _rune_detect_ptr->img_Init(_src);
    _rune_detect_ptr->setBlades();
    _rune_detect_ptr->setRotationCenter();


    if (!_start_flag)
    {
        this->Init();
        _start_flag = true;
    }
    else
    {

        Mat blade_position_matrix = Mat::zeros(_src.size(), CV_32SC1);
        int blade_num = 1;
        for (auto &&blade : _blades)
        {
            circle(blade_position_matrix, blade._center, 20,blade_num++, -1); // 对上一张图片的位置进行标记
        }
        vector<Blade> input_blades;
        vector<Point2f> input_blades_center(5);
        input_blades = _rune_detect_ptr->get_blades();
        for (auto &&blade : input_blades)
        {
            int input_center_num = blade_position_matrix.at<int>(blade._center);    //读取上一张图片的标记
            input_blades_center[input_center_num - 1] = blade._center;      //存入扇叶位置数据
            _blades[input_center_num - 1] = blade;                          //存入对应扇叶
            cout << "input_center_num = " <<input_center_num <<endl;
        }
        _blades_position_array.push_back(input_blades_center);      //存入新的扇叶中心数据
        cout << endl;

    }

    _rune_detect_last_ptr = move(_rune_detect_ptr); // 把_rune_detect_ptr传递给_rune_detect_last_ptr；
    cout << "2333" <<endl;
}

// 展示读取到的新图片
bool runeVideoProcessor::src_show(std::string winname)
{   
    imshow(winname,_src);
}

// 展示识别效果
bool runeVideoProcessor::show(std::string winname)
{
    Mat img_show = _src.clone();
    // _rune_detect_last_ptr->print_blades_contour(img_show);
    _rune_detect_last_ptr->print_circle_center(img_show);
    _rune_detect_last_ptr->print_rotationCenter(img_show);
    int num = 1;
    for(auto&& blade:_blades)
    {
        blade.print_contour(img_show,Scalar(200,100,0));
        putText(img_show,to_string(num),blade._center,FONT_HERSHEY_COMPLEX_SMALL,3.0,Scalar(100,150,200),3.0);
        num++;
    }
    imshow(winname,img_show);


    return true;
}

bool runeVideoProcessor::getStartFlag()
{
    return _start_flag;
}

int runeVideoProcessor::getNum()
{
    return _num;
}