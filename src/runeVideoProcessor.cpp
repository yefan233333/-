#include "runeVideoProcessor.h"
using namespace std;
using namespace cv;


runeVideoProcessor::runeVideoProcessor()
{
    _start_flag = false;
    _num = 0;
    _blades_polar_position_array_size = 30;
    _getSpeed_n = 10;
    _rotationCenter = Point2f(0,0);
    _blades_rotationCount_array.resize(5);
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
    if(_blades_polar_position_array.size() > _blades_polar_position_array_size)   //保证双向列表的大小恒定。
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
    this->setSpeed();
    this->setRotationFunction();

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
    Mat img_dataArray_show = Mat::zeros(Size(1900,200),CV_8UC3);
    // _rune_detect_last_ptr->print_blades_contour(img_show);
    _rune_detect_last_ptr->print_circle_center(img_show);
    // _rune_detect_last_ptr->print_rotationCenter(img_show);
    _rune_detect_last_ptr->print_blades_center(img_show);

    this->print_rotationCenter(img_show);
    this->print_blades_num(img_show);
    this->print_blades_angle(img_show);
    this->print_rotation_speed(img_show);
    // this->blades_polar_position_show(img_dataArray_show);
        this->rotation_speed_show(img_dataArray_show);

    // if(_num % 10)
    //     img_show = Scalar(51,51,51);

    imshow(winname,img_show);
    imshow("img_dataArray_show",img_dataArray_show);
    
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

//计算得到当前神符的速度
bool runeVideoProcessor::setSpeed()
{   
    if(_blades_polar_position_array.size() < _blades_polar_position_array_size)
        return false;

    vector<double> Gaussian_kernel = generateGaussianKernel(_blades_polar_position_array_size - 1 - _getSpeed_n,15);
    double speed = 0;
    for(int i =0;i<5;i++)
    {
        double angle_1 = 0;
        double angle_2 = 0;

        //高斯滤波处理
        for(int num = 0;num < Gaussian_kernel.size(); num ++)
        {
            double angle_1_add;
            double angle_2_add;
            if(_blades_polar_position_array[num][i].y > _blades_polar_position_array[num + 1 + _getSpeed_n][i].y)
            {
                angle_1_add = Gaussian_kernel[num] * _blades_polar_position_array[num][i].y;
                angle_2_add = Gaussian_kernel[num] * (_blades_polar_position_array[num + 1 + _getSpeed_n][i].y + 2 * M_PI);  
            }
            else
            {
                angle_1_add = Gaussian_kernel[num] * _blades_polar_position_array[num][i].y;
                angle_2_add = Gaussian_kernel[num] * _blades_polar_position_array[num + 1 + _getSpeed_n][i].y;  
            }
  
            angle_1 +=  angle_1_add;
            angle_2 +=  angle_2_add;

        }

        double speed_add = angle_2 - angle_1;
        speed += speed_add;

    }

    _rotationSpeed = speed;
    _rotationSpeed_array.push_back(_rotationSpeed);


    return true;
}

bool runeVideoProcessor::setRotationFunction()
{
    if(_rotationSpeed_array.size() < 500)return false;
    //获取频率
    float frequency;
    vector<float> arr_double_to_float(_rotationSpeed_array.size());
    auto && it1 = arr_double_to_float.begin();
    for(auto && data1:_rotationSpeed_array)
    {
        *(it1++) = (float)data1;
    }
    Mat dft_src(1,_rotationSpeed_array.size(),CV_32F,arr_double_to_float.data());   //为Mat对象读入数据时，记得要转为float类型
    Mat dft_dst;
    dft(dft_src,dft_dst);
    float max_frequency = 0;
    float max_dft_dst_data = 0;

    float* dft_dst_p = dft_dst.ptr<float>(0);
    dft_dst_p++;
    for(int f = 1;f < dft_dst.cols;f++)
    {
        if(max_dft_dst_data < *dft_dst_p)
        {
            max_dft_dst_data = *dft_dst_p;
            max_frequency = f;
        }
        dft_dst_p++;
    }
    frequency = max_frequency / dft_dst.cols / 2.0 / _blades_polar_position_array_size;     //得到频率。即神符在一帧内走过的周期个数。
    float omega = frequency * 2 * M_PI;         //得到角频率

    //进行系数矩阵的创建
    Mat solve_src1 = Mat::zeros(_rotationSpeed_array.size(),3,CV_32F);
    Mat solve_src2 = Mat::zeros(_rotationSpeed_array.size(),1,CV_32F);
    double* rotationSpeed_array_p = _rotationSpeed_array.data();

    for(double row = 0;row< solve_src1.rows;row++)
    {
        solve_src1.at<float>(row,0) = sin(omega * row);
        solve_src1.at<float>(row,1) = cos(omega * row);
        solve_src1.at<float>(row,2) = 1;
        solve_src2.at<float>(row,0) = (float)*(rotationSpeed_array_p++);
    }

    Mat solve_dst;
    solve(solve_src1,solve_src2,solve_dst,cv::DECOMP_SVD);
    
    float A = solve_dst.at<float>(0,0);
    float B = solve_dst.at<float>(1,0);
    float C = solve_dst.at<float>(2,0);

    float amplitude = sqrt(A*A + B*B);
    float phase_difference = atan2(B,A);
    float offset = C;

    cout << "振幅：" << amplitude <<endl;
    cout << "角频率：" <<omega <<endl;
    cout << "相位差：" <<phase_difference <<endl;
    cout << "竖直偏移量：" <<offset <<endl;



    

    return true;

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
    if(_blades_polar_position_array.size() < _blades_polar_position_array_size)
        return false;
    for(int i = 0;i<5;i++)
    {
        putText(src_show,to_string((float)_blades_polar_position_array[_blades_polar_position_array_size - 1][i].y),_blades[i]._center,2,1,Scalar(233,233,100),1);
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

bool runeVideoProcessor::print_rotation_speed(cv::Mat src_show)
{
    if(src_show.empty())
        return false;

    putText(src_show,"rotation_Speed:" + to_string(_rotationSpeed),Point2f(0,50),2,1,Scalar(200,100,0),1,LINE_8);

    return true;
}

//打印极坐标缓存区中的坐标位置，以线形图的形式
bool runeVideoProcessor::blades_polar_position_show(cv::Mat src_show)
{
    if(src_show.empty())return false;
    vector<double> array;
    for(auto position: _blades_polar_position_array)
    {
        array.push_back(position[0].y);
    }
    dataArrayShow(array,src_show);

    return true;
}

//显示旋转速度
bool runeVideoProcessor::rotation_speed_show(cv::Mat src_show)
{
    if(src_show.empty())return false;
    dataArrayShow(_rotationSpeed_array,src_show);

    return true;
}
