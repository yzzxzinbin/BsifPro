#include"headfile.h"
 int FPS = 120;//帧率
 char bool_win_imp = 'Y';
 char bool_file_out = 'Y';
 int prtmap[win_test_lonaxis][win_test_horaxis - 2];
 int loadmap[file_test_lonaxis][win_test_horaxis];

//-------------------------------驱动函数----------------------------------------
int main(void) {

	////启动前的程序准备，此处后续应该启用一个专门的函数对设置项进行文件数据读取和其他全局设置项初始化操作
	CurVis(false);
	//隐藏关闭图标
	DeleteMenu(GetSystemMenu(GetConsoleWindow(), FALSE), SC_CLOSE, MF_DISABLED);
	DrawMenuBar(GetConsoleWindow());
	if (bool_win_imp == 'Y')
		SetWindowPos(GetConsoleWindow(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_DRAWFRAME);

	/*
	HWND hWnd = GetConsoleWindow(); //获得cmd窗口句柄
	RECT rc;
	GetWindowRect(hWnd, &rc); //获得cmd窗口对应矩形
	//改变cmd窗口风格
	SetWindowLongPtr(hWnd,
		GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX & ~WS_MINIMIZEBOX);
	//因为风格涉及到边框改变，必须调用SetWindowPos，否则无效果
	SetWindowPos(hWnd,
		NULL,
		rc.left,
		rc.top,
		rc.right - rc.left, rc.bottom - rc.top,
		NULL);
	*/

	////程序大循环起始处
startspot:
	//菜单页模块核心函数
	SetColor(2, 0);//菜单页主题样式
	WindowSet(1);  //菜单页窗体属性（菜单页窗体初始化前必须先设定主题样式）

	while (true) {
		switch (_getch()) {
		case 'Q':
		case 'q':
			return 0;
		case 'I':
		case 'i':
			MessageBoxA(GetConsoleWindow(), "Copyright 2022 ZIBIN. All rights reserved.", "BSIF 5.0.2.20220628", MB_OK | MB_ICONASTERISK);
			continue;
		case 'G':
		case 'g':
			if (TestCore() != 1)
				MessageBoxA(GetConsoleWindow(), "测试终止！", "程序提示", MB_OK | MB_ICONERROR);
			goto startspot;
			break;
		case 'S':
		case 's':
			SettingCore();
			goto startspot;
			break;
		default:
			continue;
		}
	}
	return 0;
}