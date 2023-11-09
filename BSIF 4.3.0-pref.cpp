// BASIC INTERFACE.cpp - 2020.08.28 - 2020.09.17 - 2021.09.27 - 2023.05.23
// BSIF4.x是以基本接口工程（BSIF3.x）为蓝本进行更新的测试程序
// by:yangzhouzinbin
// 版本号:4.3.0
// 2021.09.24修订：修复了右边界检查的错误 ,恢复了自由移动和攻击行为,增加了新的图形显示方案
// 2021.09.25修订：修复了连击卡顿的BUG
// 2021.09.26修订：彻底解决属性条闪烁问题
// 2021.09.27修订：调用高精度时钟计时器API实现了锁帧技术
// 2021.09.28修订：重写物理系统,可显示坐标和加速度
// 2021.09.29修订：重写设置,优化了UI,重新设计了虚拟物移动方案,增加了文件读写功能,增加了分数显示,新增运行许可证,新增作弊项
// 2023.05.23修订: 重写I/O文件流,使帧率提高至原来的10倍
// 2023.05.24修订: 完善虚拟物系统

#include <iostream>	 //标准I/O
#include <windows.h> //为使用Win API提供原型
#include <conio.h>	 //为_kbhit()和非缓冲输入提供原型
#include <time.h>	 //为随机操作提供原型
#include <math.h>	 //为数学计算提供支持

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

// 实体操作对象模板
struct realObjStruct
{
	int locx;			   // 水平索引
	int locy;			   // 竖直索引
	int hp = 1000;		   // HP
	int sp = 1000;		   // SP
	int timeco = 100;	   // 高度参量
	int redir = 100;	   // 朝向
	int veh = 0;		   // 速度
	int acc = (-1) * GRAV; // 合成加速度
	short flag_sec = 0;
};
// 虚拟物对象模板
struct visObjStruct
{
	int lx;
	int ly;		   // 坐标
	int dir;	   // 朝向
	int att;	   // 伤害值
	int speed;	   // 速度（暂时无用)
	int len;	   // 生存周期
	int exist = 0; // 存在标识
	int attribute; // 虚拟物属性值
};

// 测试者实体操作对象结构体
struct realObjStruct user;
// 虚拟物对象结构体数组
struct visObjStruct vis[40];

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

//-------------------------------核心模块--------------------------------------
// 返回值为成功或失败状态量
int run(void)
{
	// 初始化界面
	system("mode con cols=60 lines=12 && title BSIF-4.2.3");
	// 日志文件系统启动
	fp = fopen("data.ini", "w");
	// 图形界面启动
	for (int i = 0; i <= WIT + 4 - 1; i++)
	{
		for (int l = 0; l <= WIT - 1; l++)
			std::cout << "　";
		std::cout << std::endl;
	}
	// 光标定位至（0,0）
	setp(0, 0);
	std::cout << "\t\t\t\t\t  [ W&K 跳跃障碍物 ]" << std::endl;
	srand(time(0));
	// 初始化数据
	user.hp = 1000;
	user.sp = 1000;
	user.locx = 4;
	user.locy = 0;
	times = 0; // 此处为开局重置
	score = 0; // 此处为开局重置
	user.timeco = 400;
	int cz;			  // 操作符
	temp_user_hp = 0; // 此处为开局重置
	temp_user_sp = 0; // 此处为开局重置
	//	DWORD time_st,time_end,time_diff;	//帧率控制旧方案
	//	time_st=GetTickCount();
	LARGE_INTEGER time_union;
	double time_st, time_end, time_diff, time_freq;

	// 游戏核心循环逻辑
	while (1)
	{
		// a:
		/*
		自动处理机制
		every round stay for sleep_time
		以帧率为前提的操作
		*/

		QueryPerformanceFrequency((LARGE_INTEGER *)&time_union); // 调用精确时钟API
		time_freq = (double)time_union.QuadPart;
		QueryPerformanceCounter((LARGE_INTEGER *)&time_union);
		time_st = (double)time_union.QuadPart;
		// time_st=GetTickCount();
		// Sleep(sleep_time);

		// 每帧增加能量1点
		if (user.sp < 1000)
			user.sp += 1;
		// 一千帧恢复HP
		if (times % 100 == 0 && user.hp + 100 <= 1000)
		{
			user.hp += 100;
		}

		//		/*
		if (times == 10000)
		{
			setp(20, 4);
			std::cout << "你成功的通关了,恭喜你！";
			std::cout.flush();
			Sleep(2000);
			return 1;
		}
		//		*/

		// 竖直重力物理系统

		user.timeco += user.veh;
		user.veh += user.acc;

		if (user.timeco < 200 && user.veh < 0)
		{
			user.timeco += user.veh;
			user.veh = 0;
			user.flag_sec = 0;
		}
		if (user.timeco >= (HIG * 100)) // 高度阈值限制
			user.timeco = ((HIG * 100) - 1);
		if (user.timeco < 100)
			user.timeco = 150;

		user.locy = user.timeco / 100;

		//------响应输入层------
		if (_kbhit())
		{
			cz = getch();
			switch (cz)
			{
			case 75:
			case 107:
			case 87:
			case 119:								 // 上
				if (user.timeco > ((HIG * 100) - 1)) // 高度出界
					break;							 // break跳出switch
				if (user.timeco < 200 && user.flag_sec == 0 && user.sp >= 80)
				{
					user.veh = 30;
					user.sp -= 80;
					user.flag_sec = 1;
					// system("pause");
				}
				if (user.flag_sec == 1 && user.veh < 0 && user.sp >= 150)
				{
					user.veh = 25;
					user.sp -= 150;
					user.flag_sec = 2;
				}
				break;

			case 65:
			case 97: // 左
				if (user.locx - 1 <= -1 || tdimg[user.locy][user.locx] == 2)
					continue;
				user.locx -= 1;
				user.redir = 97;
				break;

			case 68:
			case 100: // 右
				if (user.locx + 1 >= 30 || tdimg[user.locy][user.locx] == 2)
					continue;
				user.locx += 1;
				user.redir = 100;
				break;

			case 106: // 攻
			case 74:
			{
				if (user.sp < 50)
					goto c;
				for (int i = 0; i <= 39; i++)
				{ // 遍历虚拟表
					if (vis[i].exist == 0)
					{ // 检查i元素是否为有效信息
						switch (user.redir)
						{							 // 根据方向创建虚拟物
						case 97:					 // 左
							if (user.locx - 1 <= -1) // 超出0-29边界
								goto c;
							vis[i].lx = user.locx;
							vis[i].ly = user.locy;
							break;

						case 100:				 // 右
							if (user.locx >= 29) // 超出0-29边界
								goto c;
							vis[i].lx = user.locx;
							vis[i].ly = user.locy;
							break;
						}
						vis[i].dir = user.redir; // 根据方向创建虚拟物
						vis[i].exist = 1;
						vis[i].att = 30;
						vis[i].len = 20;
						vis[i].speed = 1;
						vis[i].attribute = 4;
						user.sp -= 50;
						selfcheck = 0; // 创建成功,本帧跳过重叠检查
						goto c;		   // 跳出遍历虚拟表
					}				   // 遍历虚拟表,如果是空结构则写入
					if (i == 39)	   // 没有空结构可以写入
						MessageBoxA(GetConsoleWindow(), "虚拟表已经达到上限,请调试！", "警告", MB_ICONERROR);
				} // 结束遍历虚拟表
			}	  // 74分支结束
			break;

			case ' ':
			{ // 空格键暂停
				setp(WIT - 6, (HIG + 2) / 2);
				std::cout << "按任意键继续";
				std::cout.flush();
				int ch = getch();
				if (ch == '+')
					user.hp = 1000;
			}
			break;

			default:
				goto c; // 其他按键顺序执行;
			}			// switch按键选择结束
		}				// 响应按键结束,此时实体位置已经与上一次img信息存在差异
	c:
		//------自动处理层------
		// 虚拟物自检
		for (int i = 0; i <= 39; i++)
		{
			if (selfcheck == 0)
			{
				if (i == 39)
					selfcheck = 1;
				goto d;
			}
			if (vis[i].exist == 1)
			{
				fprintf(fp, "%d|%d|%d|%d|%d|%d|%d", i, vis[i].ly, vis[i].lx, times, (int)FPS, (int)user.timeco, score);
				fputc('\n', fp);
				//  检查表中虚拟物存在,对本次玩家操作后进行安全性检查（因为实体方位可能改变）
				if (tdimg[vis[i].ly][vis[i].lx] == 1)
				{
					// 检查当前位置
					user.hp -= vis[i].att;
					if (user.hp > 1000)
						user.hp = 1000;
					com_clear_vis
				}
				// else if(tdimg[vis[i].ly][vis[i].lx-1]==2)
				// {}
			d:
				// 虚拟物移动
				switch (vis[i].dir)
				{
				case 97:											  // 左
					if (vis[i].lx - 1 <= -1 || (vis[i].len - 1) == 0) // 边界销毁和生存检查
						com_clear_vis else if (tdimg[vis[i].ly][vis[i].lx - 1] == 1)
						{ // 方向实体检查
							user.hp -= vis[i].att;
							if (user.hp > 1000)
								user.hp = 1000;
							com_clear_vis
						}
					else if (rand() % vis[i].speed == 0)
					{
						vis[i].lx -= 1; // 无实体按既定方向移动
						vis[i].len--;
						tdimg[vis[i].ly][vis[i].lx - 1] = tdimg[vis[i].ly][vis[i].lx] = 5;
					}
					// else if(tdimg[vis[i].ly][vis[i].lx-1]==2)
					// {}
					break;

				case 100: // 右
					if (vis[i].lx + 1 >= WIT || (vis[i].len - 1) == 0)
						com_clear_vis else if (tdimg[vis[i].ly][vis[i].lx + 1] == 1)
						{
							user.hp -= vis[i].att;
							com_clear_vis
						}
					else if (tdimg[vis[i].ly][vis[i].lx] == 5)
					{
						for (int index_vis = 0; index_vis <= 39; index_vis++)
						{
							if ((vis[index_vis].lx == vis[i].lx) && (vis[index_vis].ly == vis[i].ly) && vis[index_vis].exist == 1 && i != index_vis)
							{
								if (vis[i].ly != 1)
								{
									score += 50;
								}
								vis[i].exist = vis[i].dir = vis[i].ly = vis[i].lx = vis[i].att = vis[i].len = vis[i].speed = vis[index_vis].exist = vis[index_vis].dir = vis[index_vis].ly = vis[index_vis].lx = vis[index_vis].att = vis[index_vis].len = vis[index_vis].speed = 0;
							}
							else if ((vis[index_vis].lx == vis[i].lx + 1) && (vis[index_vis].ly == vis[i].ly) && vis[index_vis].exist == 1 && i != index_vis)
							{
								if (vis[i].ly != 1)
								{
									score += 50;
								}
								vis[i].exist = vis[i].dir = vis[i].ly = vis[i].lx = vis[i].att = vis[i].len = vis[i].speed = vis[index_vis].exist = vis[index_vis].dir = vis[index_vis].ly = vis[index_vis].lx = vis[index_vis].att = vis[index_vis].len = vis[index_vis].speed = 0;
							}
						}
					}
					else if (tdimg[vis[i].ly][vis[i].lx + 1] == 5)
					{
						for (int index_vis = 0; index_vis <= 39; index_vis++)
						{
							if ((vis[index_vis].lx == vis[i].lx + 1) && (vis[index_vis].ly == vis[i].ly) && vis[index_vis].exist == 1 && i != index_vis)
							{
								if (vis[i].ly != 1)
								{
									score += 50;
								}
								vis[i].exist = vis[i].dir = vis[i].ly = vis[i].lx = vis[i].att = vis[i].len = vis[i].speed = vis[index_vis].exist = vis[index_vis].dir = vis[index_vis].ly = vis[index_vis].lx = vis[index_vis].att = vis[index_vis].len = vis[index_vis].speed = 0;
							}
							else if ((vis[index_vis].lx == vis[i].lx + 2) && (vis[index_vis].ly == vis[i].ly) && vis[index_vis].exist == 1 && i != index_vis)
							{
								if (vis[i].ly != 1)
								{
									score += 50;
								}
								vis[i].exist = vis[i].dir = vis[i].ly = vis[i].lx = vis[i].att = vis[i].len = vis[i].speed = vis[index_vis].exist = vis[index_vis].dir = vis[index_vis].ly = vis[index_vis].lx = vis[index_vis].att = vis[index_vis].len = vis[index_vis].speed = 0;
							}
						}
					}
					else if (rand() % vis[i].speed == 0)
					{
						vis[i].lx += 1;
						vis[i].len--;
					}
					// else if(tdimg[vis[i].ly][vis[i].lx-1]==2)
					// {}
					break;
				} // 确认虚拟物方向
			}	  // 虚拟物是否存在
		}		  // 虚拟表自检for循环结束
		// b:
		// 虚拟物自动生成
		creat_new_obj();

		// 图形界面屏幕映射
		led();

		//--结束条件判断
		if (user.hp <= 0)
			return 0;

	// 帧率
	x:  //time_end=GetTickCount();
		QueryPerformanceCounter(&time_union);
		time_end = (double)time_union.QuadPart;
		time_diff = (double)(time_end - time_st) / time_freq; // 时间差值 单位：次数
		FPS = 1000.0 / (time_diff * 1000.0);

		while ((int)FPS > 3000) // 帧率锁定60FPS
			goto x;

		for (int i = 0; i <= 39; i++)
		{
			if (vis[i].exist == 1 && vis[i].lx == user.locx && user.locy > vis[i].ly)
				score += (user.locy - vis[i].ly);
		}
	} // while结束
} // run函数结束

//-----------------------------图形数据映射与显示------------------------------------
void led(void)
{
	//------映射界面层------
	// 清空映射组
	for (int y = HIG - 1; y >= 0; y--)
		for (int x = 0; x <= WIT - 1; x++)
			if (tdimg[y][x] != 3)
				tdimg[y][x] = 0;
	// 虚拟物映射
	for (int i = 0; i <= WIT - 1; i++)
	{
		if (vis[i].exist == 1)
		{
			tdimg[vis[i].ly][vis[i].lx] = vis[i].attribute;
		}
	}
	// 地面映射
	for (int x = 0; x <= WIT - 1; x++)
		tdimg[0][x] = 3; // 完整方块
	// 其他实体映射
	// 控制者实体映射
	tdimg[user.locy][user.locx] = 1;

	//------界面显示层------
	// 0 空气
	// 1 操作者
	// 2 其他实体
	// 3 地面线
	// 4 虚拟物
	// 5 阻挡物

	// 光标重定位
	setp(0, 0);

	// 参数打印
	std::cout.precision(2);
	std::cout << "Y:" << user.locy << "  "
			  << "X:" << user.locx << "  "
			  << "TIMES:" << times << "  " << std::endl
			  << "FPS:" << (int)FPS << " ";
	std::cout << "Acc：" << user.acc << " | " << user.veh << "  "
			  << "SCORE:" << score << "     " << std::endl;

	// 中央运动区打印
	for (int y = HIG - 1; y >= 0; y--)
	{
		for (int x = 0; x <= WIT - 1; x++)
		{
			if (tdimg[y][x] == 1)
			{
				
				std::cout << "♀";
				std::cout.flush();
			}
			else if (tdimg[y][x] == 0)
				std::cout << "  ";
			else if (tdimg[y][x] == 3 && times == 0)
			{
				//设置为绿色
				// std::cout.flush();
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
				std::cout << "█ ";
				std::cout.flush();
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			}
			else if (tdimg[y][x] == 5)
			{
				// std::cout.flush();
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE);
				std::cout << "█";
				std::cout.flush();
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			}
			else if (tdimg[y][x] == 4)
			{
				std::cout << "≡";
				std::cout.flush();
			}
			else if (tdimg[y][x] == 2)
			{
				
				std::cout << "++";
				std::cout.flush();
			}
		}
		std::cout << std::endl;
	}
	times++;

	// 能量打印
	if (temp_user_sp != user.sp)
	{
		int index;
		std::cout.flush();
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		setp(0, HIG + 2); // 提供20个空位 中央区+2+1
		std::cout << "SP:|";
		for (index = 1; index * 50 <= user.sp; index++)
			std::cout << "-";
		for (int i2 = 1; i2 <= 20 - (index - 1); i2++) // index-1是因为上一个for的结尾+1
			std::cout << " ";
		std::cout << "|";
		std::cout.flush();
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		temp_user_sp = user.sp;
	}

	// 血条打印
	if (temp_user_hp != user.hp)
	{
		int index;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
		std::cout.flush();
		setp(32, HIG + 2); // 提供20个空位
		std::cout << "HP:|";
		for (index = 1; index * 50 <= user.hp; index++)
			std::cout << "-";
		for (int i2 = 1; i2 <= 20 - (index - 1); i2++) // index-1是因为上一个for的结尾+1
			std::cout << " ";
		std::cout << "|";
		std::cout.flush();
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		temp_user_hp = user.hp;
	}
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
	std::ios::sync_with_stdio(false);
	std::cin.tie(0);
	std::cout.tie(0);
	SetConsoleCP(65001);
	SetConsoleOutputCP(65001);
	SetWindowPos(GetConsoleWindow(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_DRAWFRAME);
//	I/O同步关闭
d:
	// 控制台参数标题设置
	char cmd[100];
	sprintf(cmd, "mode con cols=%d lines=%d && title BSIF/%s ", 28, 9, __DATE__);
	system(cmd);
	system("cls");

	// 运行许可证
	//	int license=0;
	//	license=time(0);
	//	//1685025682
	//	if(license>1684942800) {
	//		setp(0, 3);
	//		std::cout << "\n\n---------BSIF-4.3.0---------\n    \t  正在自检\n    \t   - 5% -\n█==\t\t\t  █" << std::endl;
	//		MessageBox(NULL,L"\n    此拷贝程序为 BSIF4.3.0 测试版,仅用于测试程序稳定性,获取正式版请联系作者\n\n\n\t\t\t    >运行许可截止<\t",L" BSIF STARTING INFO",MB_OK);
	//		exit(0);
	//	}

	// 初始化菜单窗口
	prtsta();
	setp(0, 0);
	std::cout << "\n\n---------BSIF-4.3.0---------    \t  1. 测试\n    \t  2. 关于\n    \t  S. 设置\n    \t  Q. 退出";
	std::cout.flush();
	prtsqu(27, 8);

	while (true)
	{
		switch (_getch())
		{
		case 'Q':
		case 'q':
			return 0;
		case '2':
			MessageBoxA(GetConsoleWindow(), "Copyright 2023 Zhou Xu.All rights reserved.", " BSIF 4.3.0 ALPHA", MB_OK | MB_ICONASTERISK);
			continue;
		case '1':
			if (run() != 1)
				MessageBoxA(GetConsoleWindow(), "你失败了！", "LOSE", MB_OK | MB_ICONERROR);
			goto d;
		case 'S':
		case 's':
			system("cls");
			settings(1);
			goto d;
			break;
		default:
			continue;
		}
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

//----------------------------虚拟物随机参数设定---------------------------------
void creat_new_obj(void)
{
	if (rand() % (sleep_time * 5) == 0)
	{
		for (int i = 0; i <= 39; i++)
		{
			if (vis[i].exist == 0)
			{
				vis[i].lx = WIT - 1;
				// vis[i].ly=rand()%8+1;
				if (rand() % 8 <= 3)
				{
					vis[i].ly = 1;
				}
				else
					vis[i].ly = rand() % 7 + 2;
				vis[i].dir = 97;
				vis[i].speed = 2;
				vis[i].len = WIT;
				vis[i].exist = 1;
				vis[i].att = 50;
				vis[i].attribute = 5;
				return;
			}
		}
	}
	if (rand() % (sleep_time * 30) == 0)
	{
		for (int i = 0; i <= 39; i++)
		{
			if (vis[i].exist == 0)
			{
				vis[i].lx = WIT - 1;
				vis[i].ly = rand() % 8 + 1;
				vis[i].dir = 97;
				vis[i].speed = 1;
				vis[i].len = WIT;
				vis[i].exist = 1;
				vis[i].att = -100;
				vis[i].attribute = 2;
				return;
			}
		}
	}
}

void settings(int pro)
{
	int inputc;
	int index = 0;
	while (1)
	{
		if (_kbhit())
		{						   // 若有按键
			inputc = getch();	   // 按键识别
			if ((int)inputc == 77) // 索引下一个
				index++;
			else if ((int)inputc == 75) // 索引上一个
				index--;
			else if (inputc == '\n')
				system("pause");
			setp(2, 1);
			if (index == 1)
			{
				com_set_green
						std::cout
					<< "休眠设置       (回车进入)\n";
				com_set_white
			}
			else
			{
				std::cout << "休眠设置                  \n";
			}
			setp(2, 2);
			if (index == 2)
			{
				com_set_green
						std::cout
					<< "帧率设置       (回车进入)\n";
				com_set_white
			}
			else
			{
				std::cout << "帧率设置                  \n";
			}
		}
	}
}

int turn_num(void)
{
	bool condition = 0;
	int numarry[5];
	int ary_ch;		   // 数组符号
	int ary_index = 0; // 数组索引
	int num = 0;	   // 生成数
	while (1)
	{
		if (_kbhit())
		{
			ary_ch = getch();
			switch ((int)ary_ch)
			{
			case 1:
				numarry[ary_index] = 1;
				ary_index++;
				break;
			case 2:
				numarry[ary_index] = 2;
				ary_index++;
				break;
			case 3:
				numarry[ary_index] = 3;
				ary_index++;
				break;
			case 4:
				numarry[ary_index] = 4;
				ary_index++;
				break;
			case 5:
				numarry[ary_index] = 5;
				ary_index++;
				break;
			case 6:
				numarry[ary_index] = 6;
				ary_index++;
				break;
			case 7:
				numarry[ary_index] = 7;
				ary_index++;
				break;
			case 8:
				numarry[ary_index] = 8;
				ary_index++;
				break;
			case 9:
				numarry[ary_index] = 9;
				ary_index++;
				break;
			case 0:
				numarry[ary_index] = 0;
				ary_index++;
				break;
			case '\n':
				condition = 1;
				goto p;
			default:
				continue;
			}
		}
		else
			return 0;
	}
p:
	if (condition)
	{
		for (int i = 1; i <= 5 - 1; i++)
			num += (pow(10, i) * numarry[i]);
		std::cout << num;
		return num;
	}
	return 0;
}

void prtsqu(int x, int y)
{
	//	for(int a=0;a<=x;a++)
	//		for(int b=0;b<=y;b++)
	//			std::cout<<" ";
	com_set_green
		setp(0, 0);
	for (int i = 0; i <= x; i++)
		std::cout << '=';
	std::cout.flush();
	setp(0, y);
	for (int i = 0; i <= x; i++)
		std::cout << '=';
	std::cout.flush();
	for (int i = 1; i < y + 0; i++)
	{
		setp(0, i);
		std::cout << "|";
		std::cout.flush();
	}
	for (int i = 1; i < y + 0; i++)
	{
		setp(x, i);
		std::cout << "|";
		std::cout.flush();
	}
	com_set_white
		setp(0, 0);
}
