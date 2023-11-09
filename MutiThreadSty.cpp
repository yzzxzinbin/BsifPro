#include <iostream>	 //标准I/O
#include <windows.h> //为使用Win API提供原型
#include <conio.h>	 //为_kbhit()和非缓冲输入提供原型
#include <time.h>	 //为随机操作提供原型
#include <math.h>	 //为数学计算提供支持
#include <atomic>	 //为原子操作提供原型
#include <thread>	 //为线程操作提供原型
#include <pthread.h> //为线程操作提供原型
#define com_clear_vis vis[i].exist = vis[i].dir = vis[i].ly = vis[i].lx = vis[i].att = vis[i].len = vis[i].speed = 0;
#define com_set_green SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
#define com_set_white SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#define HIG 9
#define WIT 30
#define GRAV 1

// 打印开始窗口
void prtsta(void);
// 启动测试
int run(void);
// 光标定位
static void setp(int x, int y);
// 映射画面
void led(void);
// 虚拟物自动生成
void creat_new_obj(void);
// 设置模块
void settings(int);
// 字符转整数函数
int turn_num(void);
// 窗体UI
void prtsqu(int x, int y);


// 原子变量
std::atomic<int> myAtomicInt = 1;

// 窗口数据二维数组
int tdimg[HIG][WIT]; // 0-5  0-29
// 测试时间刻
int times = 0;
// 比对血量参数
int temp_user_hp;
// 比对能量参数
int temp_user_sp;
// 每帧休眠（已废弃）
int sleep_time = 8;
// 跳过自检
int selfcheck = 4;
// 帧率
double FPS;
// 总得分
int score = 0;
// 日志文件指针
FILE *fp = NULL;

// 编写一个并行线程的主函数,作用是打印某一原子变量的值并＋1
void thread_fun(void)
{
	HANDLE threadHandle = GetCurrentThread();
	// 将线程绑定到第一个CPU核心上
	DWORD_PTR threadAffinityMask = 9;
	DWORD_PTR previousAffinityMask = SetThreadAffinityMask(threadHandle, threadAffinityMask);

	// 线程开始
	std::cout << myAtomicInt;
	while (myAtomicInt <= 10000000)
	{
		// 原子操作
		std::cout << " 附加线程:  " << myAtomicInt << "\n";
		myAtomicInt++;
		// 休眠
	}
	// 线程结束
	return;
}

//-----------------------------菜单窗口初始化------------------------------------
void prtsta(void)
{
	// 获得标准输出句柄
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cur_info;
	// 通过句柄获得输出参数
	GetConsoleCursorInfo(handle, &cur_info);
	// 关闭光标显示
	cur_info.bVisible = FALSE;
	// 更改参数导入操作系统
	SetConsoleCursorInfo(handle, &cur_info);
}

//-------------------------------驱动函数----------------------------------------
int main(int argc, char **argv)
{

	//	I/O同步关闭
	std::ios::sync_with_stdio(false);
	std::cin.tie(0);
	std::cout.tie(0);
	SetConsoleCP(65001);
	SetConsoleOutputCP(65001);
	SetWindowPos(GetConsoleWindow(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_DRAWFRAME);
d:
	// 控制台参数标题设置
	char cmd[100];
	sprintf(cmd, "mode con cols=%d lines=%d && title BSIF/%s ", 28, 9, __DATE__);
	system(cmd);
	system("cls");
	// 初始化菜单窗口
	prtsta();
	setp(0, 1);
	std::cout << "\n\n---------BSIF-4.2.X---------    \t  1. 测试\n    \t  2. 关于\n    \t  S. 设置\n    \t  Q. 退出";
	std::cout.flush();

	// 使用C++标准,创建新的线程
	std::thread t1(thread_fun);
	t1.detach();

	int count = 1;
	while (true)
	{
		HANDLE threadHandle = GetCurrentThread();
		// 将线程绑定到第一个CPU核心上
		DWORD_PTR threadAffinityMask = 8;
		DWORD_PTR previousAffinityMask = SetThreadAffinityMask(threadHandle, threadAffinityMask);
		if (myAtomicInt != 0)
			std::cout << "主线程执行中" << count << "\n";
		count ++;
	}
	return 0;
}

//-------------------------------光标移动函数------------------------------------
static void setp(int x, int y)
{
	// 获得光标参数
	COORD position;
	// 标准输出句柄
	HANDLE handle;
	handle = GetStdHandle(STD_OUTPUT_HANDLE);
	// 更改系统光标位置
	position.X = x;
	position.Y = y;
	// 更改数据导入API
	SetConsoleCursorPosition(handle, position);
}


