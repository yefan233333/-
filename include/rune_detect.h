#pragma once
#include<iostream>
class rune_delete
{
public:
    int _num;
    rune_delete(int num):_num(num)
    {
        std::cout<<"成功设置Num = "<<_num <<std::endl;
    }
    void show();
};