////CMD通用自定义支持库头文件
#include <iostream>
#include <Windows.h>
using namespace std;

//图形类
void cui_basic_line(int x, int y, int longdata, wchar_t symbol, int direct);//起始点横纵坐标,长度,符号,方向
void cui_position_set(int x, int y);//移动光标至指定位置
void cui_square_frame(int x, int y, int lenth,int height,int model);//起始点横纵坐标,长度,高度,模式
void cui_basic_fill(int x, int y, int lenth, int height, wchar_t ch);//起始点横纵坐标,长度,高度,字符

//控制类
double cct_time_proc(char model);

//系统类
void csy_load_style(int backinfo,int foreinfo);//背景色,前景色
void csy_load_window(int x,int y,const char info[30],int backinfo,int foreinfo);//长度,高度,标题,背景色,前景色