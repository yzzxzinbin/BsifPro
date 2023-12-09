//BASIC INTERFACE.cpp
//2020.08.28 - 2020.09.17 - 2021.09.27 - 2022.06.28 - 2022.07.01 - 2022.07.05 - 2023.03.12 - 2023.04.05
//by:yangzhouzinbin
//版本号：5.0.2.230315

#pragma warning(disable : 4996) 
#pragma once
using namespace std;			//标准命名空间

#include <iostream> //标准I/O 
#include <windows.h>//为使用Win API提供原型 
#include <conio.h>  //为_kbhit()和非缓冲输入提供原型 
#include <time.h>   //为随机操作和时间控制提供原型 
#include <math.h>	//为数学计算提供支持 
#include <stdlib.h> //通用工具函数库
#include <wchar.h>  //宽字符输出
#include <locale.h> //本地化函数库

//窗口尺寸参量
#define win_menu_lonaxis 28//横长
#define win_menu_horaxis 9//纵长
#define win_test_horaxis 12//真实高度
#define win_test_lonaxis 60//真实长度
#define file_test_lonaxis 1000//文件长度

#define time_unit 10//基本时间刻(单位ms)
#define com_set_green SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_GREEN);
#define com_set_white SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY|FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);

void WindowSet(int);
int TestCore(void);
void SettingCore(void);
void SetCurPos(int x, int y);
void SetColor(UINT uFore, UINT uBack);
void CurVis(bool select);
int GetNumKeb(int limit);

//-------------------------------全局变量----------------------------------------
extern int FPS;//帧率
extern char bool_win_imp;
extern char bool_file_out;
extern int prtmap[win_test_lonaxis][win_test_horaxis - 2];
extern int loadmap[file_test_lonaxis][win_test_horaxis];
