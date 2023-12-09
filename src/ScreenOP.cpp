#include <iostream>
#include <windows.h>
using namespace std;
class ConsoleStyle
{
public:
    static void SetFont(const wchar_t *fontName, int fontSize)
    {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_FONT_INFOEX fontInfo;
        fontInfo.cbSize = sizeof(CONSOLE_FONT_INFOEX);
        GetCurrentConsoleFontEx(hConsole, FALSE, &fontInfo);

        wcscpy_s(fontInfo.FaceName, fontName);
        fontInfo.dwFontSize.X = 0;
        fontInfo.dwFontSize.Y = fontSize;

        SetCurrentConsoleFontEx(hConsole, FALSE, &fontInfo);
    }

    static void SetColor(int colorIndex, COLORREF color)
    {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFOEX screenBufferInfo;
        screenBufferInfo.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
        GetConsoleScreenBufferInfoEx(hConsole, &screenBufferInfo);

        screenBufferInfo.ColorTable[colorIndex] = color;

        SetConsoleScreenBufferInfoEx(hConsole, &screenBufferInfo);
    }
};
bool OpenANSIControlChar()
{ // 开启ANSI转义序列功能
    // 获取控制台的控制句柄
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

int main(void)
{
    // for (int x = 0; x < 25; x++)
    // {
    //     for (int y = 0; y < 25; y++)
    //         std::cout << " ";
    // }
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    CONSOLE_CURSOR_INFO cur_info;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cur_info);
    cur_info.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cur_info);
    HANDLE hOutStd = GetStdHandle(STD_OUTPUT_HANDLE);
    OpenANSIControlChar();

    // for (int index = 0; index<200000; index++)
    // {
    //     SetPosition(index % 10, index % 10);
    //     std::cout << index;
    //     //std::cout.flush();
    // }
    ConsoleStyle::SetFont(L"Consolas", 22);
    ConsoleStyle::SetColor(0, RGB(255, 0, 0)); // 红色
    ConsoleStyle::SetColor(1, RGB(0, 255, 0)); // 绿色

    std::cout << "Hello, World!" << std::endl;
    ConsoleStyle::SetFont(L"Courier New", 22);
    std::cout << "Hello, World!" << std::endl;
    // std::cout << "\033[31m"
    //           << "Hello, World!"
    //           << "\033[0m" << std::endl;
    std::cin.get();
    return 0;
}