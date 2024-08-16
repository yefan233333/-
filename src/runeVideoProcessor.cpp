#include "runeVideoProcessor.h"
using namespace std;
using namespace cv;


runeVideoProcessor::runeVideoProcessor()
{
    _start_flag = false;
    _num = 0;
    _rotationCenter = Point2f(0,0);
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

//更新旋转中心的位置
void runeVideoProcessor::setRotationCenter()
{
    double P = 1.0 / _num;
    _rotationCenter = P * _rune_detect_ptr->_rotationCenter + (1 - P) * _rotationCenter;
}

//更新扇叶的位置
void runeVideoProcessor::setBladePosition()
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
    }
    _blades_position_array.push_back(input_blades_center);      //存入新的扇叶中心数据

    //计算扇叶的极坐标位置
    vector<double> m_x;
    vector<double> m_y;
    vector<double> magnitude;
    vector<double> angle;
    vector<Point2f> blades_polar_position(5);
    for(auto&& blade_center:input_blades_center)
    {
        m_x.push_back(blade_center.x - _rotationCenter.x);  //将极坐标的中心位置设定在旋转中心
        m_y.push_back(blade_center.y - _rotationCenter.y);
    }
    cartToPolar(m_x,m_y,magnitude,angle);
    int i = 0;
    for(auto&& position:blades_polar_position)
    {
        position.x = magnitude[i];
        position.y = angle[i];
        i++;
    }

    _blades_polar_position_array.push_back(blades_polar_position);
    if(_blades_polar_position_array.size() > 10)   //保证双向列表的大小恒为10。
    {
        _blades_polar_position_array.pop_front();
    }
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

    this->setRotationCenter();

    if (!_start_flag)
    {
        this->Init();
        _start_flag = true;
    }
    else
    {
        this->setBladePosition();

    }

    _rune_detect_last_ptr = move(_rune_detect_ptr); // 把_rune_detect_ptr传递给_rune_detect_last_ptr；
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
    // _rune_detect_last_ptr->print_rotationCenter(img_show);
    _rune_detect_last_ptr->print_blades_center(img_show);
    this->print_rotationCenter(img_show);
    this->print_blades_num(img_show);
    this->print_blades_angle(img_show);

    imshow(winname,img_show);

    // for(auto&& blades_polar_position:_blades_polar_position_array)
    // {
    //     cout << blades_polar_position <<" ";
    // }
    // cout << endl;


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

//计算得到神符的速度
void runeVideoProcessor::setSpeed()
{   

}

bool runeVideoProcessor::print_rotationCenter(cv::Mat src_show)
{
    if(src_show.empty())
        return false;
    
    circle(src_show,_rotationCenter,10,Scalar(150,100,0),-1);
    return true;
}

bool runeVideoProcessor::print_blades_angle(cv::Mat src_show)
{
    if(src_show.empty())
        return false;
    if(_blades_polar_position_array.size() < 10)
        return false;
    for(int i = 0;i<5;i++)
    {
        putText(src_show,to_string((float)_blades_polar_position_array[9][i].y),_blades[i]._center,2,1,Scalar(233,233,100),1);
    }
    return true;
}

bool runeVideoProcessor::print_blades_num(cv::Mat src_show)
{
    if(src_show.empty())
        return false;
    int num = 1;
    for(auto&& blade:_blades)
    {
        blade.print_contour(src_show,Scalar(200,100,0));
        putText(src_show,to_string(num),blade._center,FONT_HERSHEY_COMPLEX_SMALL,3.0,Scalar(100,150,200),3.0);
        num++;
    }
    return true;
}
