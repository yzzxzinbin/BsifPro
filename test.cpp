// 测试Windows控制台输出特殊字符的程序
// 使用Windows API实现光标的定位等操作
#include <windows.h>
#include <iostream>
bool SetPosition(int x, int y)
{
    // 如果x,y为-1,则位置回退一格
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    COORD pos;
    if (x == -1)
        pos.X = csbi.dwCursorPosition.X - 1;
    else
        pos.X = x;
    if (y == -1)
        pos.Y = csbi.dwCursorPosition.Y - 1;
    else
        pos.Y = y;

    return SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
int main()
{

    // 关闭IO同步
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);

    // 定义窗口句柄变量,包括窗口句柄,标准输出句柄等
    HWND hwnd = GetConsoleWindow();
    HANDLE StdOutH = GetStdHandle(STD_OUTPUT_HANDLE);

    // 代码页设置为UTF-8,增加对特殊字符的支持
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    // 窗口置顶
    SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_DRAWFRAME);

    // 关闭光标显示
    CONSOLE_CURSOR_INFO cur_info;
    cur_info.bVisible = FALSE;
    SetConsoleCursorInfo(StdOutH, &cur_info);

    // 游戏主循环
    while (true)
    {
        SetPosition(2, 2);
        std::cout << "█ █ ≡1";
        std::cout.flush();
    }
    return 0;
}