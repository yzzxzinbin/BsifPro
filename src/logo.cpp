#include <windows.h>
#include <vector>
#include <array>
#include <iostream>
#include <fstream>
#include <random>
#include <filesystem>
#include <conio.h>
#include <mmsystem.h>
#define FPSLIMIT 60
#define WIN_MENU_LONAXIS 64 // 横长
#define WIN_MENU_HORAXIS 25 // 纵长
HANDLE hOutStd;
HWND hwnd;
bool SetPosition(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    return SetConsoleCursorPosition(hOutStd, coord);
}
bool OpenANSIControlChar()
{
    // 开启ANSI转义序列功能
    HANDLE hStd = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hStd == INVALID_HANDLE_VALUE)
    {
        return false;
    }
    // 获取控制台模式
    DWORD dwMode = 0;
    if (!GetConsoleMode(hStd, &dwMode))
    {
        return false;
    }
    // 增加控制台模式的选项：启用虚拟终端
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!SetConsoleMode(hStd, dwMode))
    {
        return false;
    }
    return true;
}
void InitMainDrive(void)
{
    // 定义窗口句柄变量,包括窗口句柄,标准输出句柄等
    hwnd = GetConsoleWindow();
    hOutStd = GetStdHandle(STD_OUTPUT_HANDLE);

    OpenANSIControlChar();

    // 关闭光标显示
    CONSOLE_CURSOR_INFO cur_info;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cur_info);
    cur_info.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cur_info);

    // 代码页设置为UTF-8,增加对特殊字符的支持
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    // int cols = WIN_MENU_LONAXIS; // 指定列数
    // int lines = WIN_MENU_HORAXIS - 1; // 指定行数
    // std::string command = "mode con cols=" + std::to_string(cols) + " lines=" + std::to_string(lines);
    // system(command.c_str());

    SMALL_RECT windowSize = {0, 0, WIN_MENU_LONAXIS - 1, WIN_MENU_HORAXIS - 2};
    SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, &windowSize);
    COORD bufferSize = {WIN_MENU_LONAXIS, WIN_MENU_HORAXIS - 1};
    SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), bufferSize);

    SetPosition(0, 0);
    std::string logotxt = R"(
           ___           ___                       ___     
          /\  \         /\  \          ___        /\  \
         /::\  \       /::\  \        /\  \      /::\  \
        /:/\:\  \     /:/\ \  \       \:\  \    /:/\:\  \
       /::\ \:\__\   _\:\ \ \  \      /::\__\  /::\ \:\  \
      /:/\:\ \:|__| /\ \:\ \ \__\  __/:/\/__/ /:/\:\ \:\__\
      \:\ \:\/:/  / \:\ \:\ \/__/ /\/:/  /    \/__\:\ \/__/
       \:\ \::/  /   \:\ \:\__\   \::/__/          \:\__\
        \:\/:/  /     \:\/:/  /    \:\__\           \/__/  
         \::/__/       \::/  /      \/__/                  
          ~~            \/__/                              
     )";
    std::cout << logotxt;
    SetPosition(8, 12);
    std::cout << "\033[33m\033[1m\033[46m"
              << "\n--------------------------BSIF 6.0.1---------------------------"
              << "\033[36m\033[1m\033[40m"
              << "\n\n    \t\t\t   1. 测试\n\n    \t\t\t   2. 关于\n\n    \t\t\t   S. 设置\n\n    \t\t\t   Q. 退出"
              << "\033[0m";
    SetPosition(0, 0);
    for (int i = 0; i <= WIN_MENU_LONAXIS - 1; i++)
        std::cout << "\033[32m" << '=' << "\033[0m";
    SetPosition(0, WIN_MENU_HORAXIS - 2);
    for (int i = 0; i <= WIN_MENU_LONAXIS - 1; i++)
        std::cout << "\033[32m" << '=' << "\033[0m";
    for (int i = 0; i <= WIN_MENU_HORAXIS - 2; i++)
    {
        SetPosition(0, i);
        std::cout << "|";
    }
    for (int i = 0; i <= WIN_MENU_HORAXIS - 2; i++)
    {
        SetPosition(WIN_MENU_LONAXIS - 1, i);
        std::cout << "|";
    }
    SetPosition(0, 0);
}

int main(void)
{
    InitMainDrive();

    while (true)
    {
        if (GetAsyncKeyState('1') & 0x8000) // 检查S键是否被按下，并且地图数据中相应位置为1
        {
        }
        if (GetAsyncKeyState('2') & 0x8000) // 检查S键是否被按下，并且地图数据中相应位置为1
        {
            MessageBoxA(GetConsoleWindow(), "Copyright 2023 Zhou Xu.All rights reserved.", " BSIF 4.3.0 ALPHA", MB_OK | MB_ICONASTERISK);
            continue;
        }
        if (GetAsyncKeyState('Q') & 0x8000) // 检查S键是否被按下，并且地图数据
        {
            break;
        }
    }
    std::cin.get();
    return 0;
}