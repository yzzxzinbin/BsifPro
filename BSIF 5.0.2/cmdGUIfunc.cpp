//CMD通用自定义支持库
//UPTATE:2023.3.12

#include "cmdGUIhead.h"

void cui_position_set(int x, int y)
{
	//作用等同于SetCurPos()

	//获得光标参数
	COORD position{};
	//标准输出句柄
	HANDLE handle;
	handle = GetStdHandle(STD_OUTPUT_HANDLE);
	//更改光标数据
	position.X = x;
	position.Y = y;
	//更改数据导入API
	SetConsoleCursorPosition(handle, position);
}

void cui_basic_line(int x, int y, int longdata, wchar_t symbol, int direct)
{
	/*
		使用前提:
		本函数调用时候如果传入宽字符参数,必须事先使用
		setlocale(LC_ALL, "zh-CN");
		进行本地化语言设置

		direct参数可选项:
		0:竖向输出
		1:横向输出
	*/

	//定位至目标位置
	register COORD position{};
	register HANDLE handle;

	handle = GetStdHandle(STD_OUTPUT_HANDLE);
	position.X = x;
	position.Y = y;
	SetConsoleCursorPosition(handle, position);

	//判断工作模式
	if (direct)
	{
		for (register int i = 1; i <= longdata; i++)
			putwchar(symbol);
	}
	else
	{
		for (register int n = 1; n <= longdata; n++, y++)
		{
			position.Y = y;
			SetConsoleCursorPosition(handle, position);
			wcout << symbol;
		}
	}
}

void cui_square_frame(int x, int y, int lenth, int height, int model)
{
	/*
	窗口参数:
	模式0外部识别区: x,y ---------- x+lenth-1,y
					 |                 |
			   x,y+height-1 ----- x+lenth-1,y+height-1


	*/
	cui_position_set(x, y);

	switch (model)
	{
	case 0:
		cui_basic_line(x, y, lenth, '=', 1);
		cui_basic_line(x, y + height - 1, lenth, '=', 1);
		cui_basic_line(x, y + 1, height - 2, L'║', 0);
		cui_basic_line(x + lenth - 2, y + 1, height - 2, L'║', 0);
		break;
	}

	switch (model)
	{
	case 0:
		cui_position_set(x, y);
		wcout << L"╔";
		cui_position_set(x + lenth - 2, y);
		wcout << L"╗";
		cui_position_set(x, y + height - 1);
		wcout << L"╚";
		cui_position_set(x + lenth - 2, y + height - 1);
		wcout << L"╝";
	}
}

void cui_basic_fill(int x, int y, int lenth, int height, wchar_t ch)
{
	COORD position{};
	HANDLE handle;
	handle = GetStdHandle(STD_OUTPUT_HANDLE);
	position.X = x;
	position.Y = y;

	for (int h = 1; h <= height; h++) {

		position.Y = y + h - 1;
		SetConsoleCursorPosition(handle, position);
		for (int l = 1; l <= lenth; l++)
			wcout << ch;
	}
}

double cct_time_proc(char model)
{
	/*
	精确时控自定义API
	工作模式:f 获取频率
			 s 起始计数器
			 e 末尾计数器
			 d 计算差值
			 z 以FPS显示
			 i 静态量重置
	*/
	LARGE_INTEGER time_union;
	static double time_freq, time_st, time_end ,time_diff;

	switch (model)
	{
	case 'f':
		QueryPerformanceFrequency((LARGE_INTEGER*)&time_union);
		return time_freq = (double)time_union.QuadPart;
	case 's':
		QueryPerformanceCounter((LARGE_INTEGER*)&time_union);
		return time_st = (double)time_union.QuadPart;
	case 'e':
		QueryPerformanceCounter((LARGE_INTEGER*)&time_union);
		return time_end = (double)time_union.QuadPart;
	case 'd':
		return time_diff = (double)(time_end - time_st) /time_freq;//单位 ms
	case 'z':
		return  (1000.0/(time_diff * 1000.0));
	case 'i':
		return time_freq = time_st = time_end = time_diff = 1;
	default :
		return 0;
	}
}

void csy_load_style(int backinfo,int foreninfo){
/*
CMD中关于颜色的数学表示
   0 = 黑色       8 = 灰色
   1 = 蓝色       9 = 淡蓝色
   2 = 绿色       A = 淡绿色
   3 = 浅绿色     B = 淡浅绿色
   4 = 红色       C = 淡红色
   5 = 紫色       D = 淡紫色
   6 = 黄色       E = 淡黄色
   7 = 白色       F = 亮白色
*/
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 16 * backinfo + foreninfo);
}

void csy_load_window(int x,int y,const char pageinfo[30],int backinfo,int foreinfo){
	char cmd[100];
	sprintf(cmd, "mode con cols=%d lines=%d && title BSIF∣%s ", x ,y,pageinfo);

	system(cmd);
	csy_load_style(backinfo,foreinfo);
	system("cls");
}