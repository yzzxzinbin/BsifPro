#include"headfile.h"

//设置页模块核心函数
//设置页窗体属性，主题样式/UI等基本参数设定已经内含其中
void SettingCore(void) {

	int inputc, preputc;
	int index = -1;

setc_success_setting:
	system("color E0");//窗口背景填充（GUI）
	WindowSet(3);//窗体属性设置

	index = -1;
	cout << "                 [设置]\n  ←→键选择项目  [Enter]键进入相关设置";

	while (1) {
		{
			//非停滞检测按键
			//读取方向键键盘扫描码（十六位）需要两次getch，后一次为对应ASCII码
			preputc = getch();
			if (preputc == 224) {
				inputc = _getch();
				//按键识别
				if ((int)inputc == 77)//索引下一个
				{
					index++;

				}
				else if ((int)inputc == 75)//索引上一个
					index--;
				//刷出主显示区背景色并清除上一响应残留文本（GUI）
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_INTENSITY | BACKGROUND_BLUE);
				for (int i = 2; i <= win_menu_horaxis + 8 - 2; i++)
				{
					SetCurPos(2, i);
					for (int r = 2; r <= win_menu_lonaxis + 12 - 3; r++)
					{
						cout << " ";
					}
				}

				SetCurPos(4, 3);
				cout << "帧率锁定 ";//设置项目0
				SetCurPos(4, 4);
				cout << "窗口前置 ";//设置项目1
				SetCurPos(4, 5);
				cout << "文件输出 ";//设置项目2
	//......以后可继续添加可选设置项
	// 
	//选定区域覆盖（index为索引）
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_RED);
				switch (index)
				{
				case 0:
					SetCurPos(2, index + 3);
					for (int r = 2; r <= win_menu_lonaxis + 12 - 3; r++)//对选定区域进行高亮显示
						cout << " ";
					SetCurPos(4, index + 3);
					cout << "帧率锁定\t\t\t" << FPS;//设置项目0
					break;
				case 1:
					SetCurPos(2, index + 3);
					for (int r = 2; r <= win_menu_lonaxis + 12 - 3; r++)//对选定区域进行高亮显示
						cout << " ";
					SetCurPos(4, index + 3);
					cout << "窗口前置\t\t\t" << bool_win_imp;//设置项目0
					break;
				case 2:
					SetCurPos(2, index + 3);
					for (int r = 2; r <= win_menu_lonaxis + 12 - 3; r++)//对选定区域进行高亮显示
						cout << " ";
					SetCurPos(4, index + 3);
					cout << "文件输出\t\t\t" << bool_file_out;//设置项目
					break;

				}//switch end

			}//方向键方法结束，enter逻辑开始
			else if (preputc == 13) {
				CurVis(true);//显示输入区域

				switch (index)
				{
				case 0:
					SetCurPos(32, index + 3);
					FPS = GetNumKeb(3);
					if (FPS > 360 || FPS < 60)
						FPS = 120;
					CurVis(false);
					goto setc_success_setting;
					break;

				case 1://not completely				
				{
					SetCurPos(32, index + 3);
					char winset_getcres = getch();
					if (winset_getcres == 'N' || winset_getcres == 'n')
					{
						bool_win_imp = 'N';
						SetWindowPos(GetConsoleWindow(), HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_DRAWFRAME);
					}
					else if (winset_getcres == 'Y' || winset_getcres == 'y')
					{
						bool_win_imp = 'Y';
						SetWindowPos(GetConsoleWindow(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_DRAWFRAME);

					}
					CurVis(false);
					goto setc_success_setting;
					break;
				}
				case 2:
					SetCurPos(32, index + 3);
					char winset_getcres = getch();
					if (winset_getcres == 'N' || winset_getcres == 'n')
						bool_file_out = 'N';
					else if (winset_getcres == 'Y' || winset_getcres == 'y')
						bool_file_out = 'Y';
					CurVis(false);
					goto setc_success_setting;
					break;
				}
				CurVis(false);
			}
			else if (preputc == ' ')
			{
				CurVis(false);
				return;
			}
		}//one hit

	}//while end

}


//测试页模块核心函数
int TestCore(void)
{
	WindowSet(2);
//启动文件系统
	FILE* mapfp;
	mapfp = fopen("map.ini", "a+");

	int mapch = 0;

//向loadmap中写入文件数据
	for (int lonindex = 0; lonindex < file_test_lonaxis; lonindex++)
	{
		for (int horindex = 0; horindex < win_test_horaxis - 2; horindex++)
		{
			mapch = loadmap[lonindex][horindex] = getc(mapfp);
		}
	}
innerwhile:
//初始化结束 时间刻启动

//输出层{线程2完成}
	for (int horindex = 0; horindex < win_test_horaxis - 2; horindex++)
	{
		for (int lonindex = 0; lonindex < win_test_lonaxis; lonindex++)
		{
			switch (loadmap[lonindex][horindex])
			{
			case 48:cout << "  ";
				break;
			case 49:cout << "█";
				break;
			case 50:cout << "";
				break;
			}
		}
	}

//新版控制台需要用半方块▌▌█

	getchar();
	return 0;
}