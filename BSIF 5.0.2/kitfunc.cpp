#include"headfile.h"

////文本设置封装函数
////参数一为前景色，参数二为背景色
//1.控制台颜色十六进制表
//0 = 黑色      8 = 灰色   1 = 蓝色      9 = 淡蓝色   2 = 绿色      A = 淡绿色   3 = 湖蓝色    B = 淡浅绿色   4 = 红色
//C = 淡红色   5 = 紫色      D = 淡紫色   6 = 黄色      E = 淡黄色   7 = 白色      F = 亮白色
void SetColor(UINT uFore, UINT uBack) {

	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handle, uFore + uBack * 0x10);

}

////光标位置封装函数
void SetCurPos(int x, int y) {

	//获得光标参数
	COORD position{};
	//标准输出句柄
	HANDLE handle;
	handle = GetStdHandle(STD_OUTPUT_HANDLE);
	//更改系统光标位置
	position.X = x;
	position.Y = y;
	//更改数据导入API
	SetConsoleCursorPosition(handle, position);

}

////光标显示封装函数
void CurVis(bool select) {

	//标准输出句柄
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cur_info;
	//通过句柄获得输出参数
	GetConsoleCursorInfo(handle, &cur_info);
	if (select == 0)
		cur_info.bVisible = FALSE;	//关闭光标
	if (select == 1)
		cur_info.bVisible = TRUE;	//关闭光标
		//更改参数导入API
	SetConsoleCursorInfo(handle, &cur_info);
}

////给定限定数，读取并返回一个以此为位数上限的数字
////附带清空输入行操作
int GetNumKeb(int limit) {

	rewind(stdin);

	int index = 0;//数组索引
	const int inputmax = 30; //具体量(上限)(元素数)
	char dealorg[inputmax] = {};//操作主题数组
	int conseq = 0;//结果数值

	while (index < limit && index < inputmax) {

		dealorg[index] = getche();
		if (dealorg[index] == '\n')
		{
			dealorg[index] = '\0';
			goto GNB_dealconseque;//缓冲区为空 跳过清空输入缓冲区
		}
		index++;
	}

	rewind(stdin);

GNB_dealconseque:
	conseq = atoi(dealorg);
	return conseq;
}

////窗体初始化封装函数
////该函数根据一个参数（0，1，2）决定使用何种窗体的属性,启动时刷新页面（cls）
////关于窗体属性的常量已在预处理区定义，本函数仅仅初始化窗体属性不包括样式属性修改，需要调用前使用setcolor函数定义
void WindowSet(int determine) {

	system("cls");//开始新窗体页面

	if (determine == 1) {
		//菜单窗体
		char cmd[100];
		sprintf(cmd, "mode con cols=%d lines=%d && title BSIF∣%s ", win_menu_lonaxis, win_menu_horaxis + 1, __DATE__);
		system(cmd);
		system("cls");

		//在XP机器上编译时需要以下注释兼容代码
		for (int a = 0; a <= win_menu_lonaxis; a++)
			for (int b = 0; b <= win_menu_horaxis; b++)
				cout << "  ";
		{

			SetCurPos(0, 0);
			cout << "\n---------BSIF-5.0.2---------\n    \t  1. 测试\n    \t  2. 关于\n    \t  S. 设置\n    \t  Q. 退出";

			SetCurPos(0, 0);
			for (int i = 0; i <= win_menu_lonaxis - 1; i++)
				cout << '=';

			SetCurPos(0, win_menu_horaxis - 2);
			for (int i = 0; i <= win_menu_lonaxis - 1; i++)
				cout << '=';

			for (int i = 0; i <= win_menu_horaxis - 2; i++) {
				SetCurPos(0, i);
				cout << "|";
			}

			for (int i = 0; i <= win_menu_horaxis - 2; i++) {
				SetCurPos(win_menu_lonaxis - 1, i);
				cout << "|";
			}

			SetCurPos(0, 0);
		}
		//以上为菜单页打印功能具体实现
	}
	else if (determine == 2) {
		//测试窗体
		char cmd[100];
		sprintf(cmd, "mode con cols=%d lines=%d && title BSIF∣%s ", win_test_lonaxis * 2, win_test_horaxis, __DATE__);
		system(cmd);
		system("cls");
	}
	else if (determine == 3) {
		//设置窗体
		char cmd[100];
		sprintf(cmd, "mode con cols=%d lines=%d && title BSIF∣%s ", win_menu_lonaxis + 12, win_menu_horaxis + 8, "设置");
		system(cmd);
		system("cls");
	}
}


////清空缓冲区封装函数
//void clearbuff(void) {
//	if (kbhit())
//		while (getchar() != '\n')
//			continue;
//}