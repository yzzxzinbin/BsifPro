// BASICINTERFACE 6Gen1 - 基本接口工程第六代
// Ver 6.1.0 - preRelease
// 2020.08.28 - 2020.09.17 - 2021.09.27 - 2023.05.23 - 2023.12.09
// 231209(6.1.0 - preRelease):更新了图形显示函数,在不损失性能的前提下新增彩色文字输出
#include <windows.h>
#include <vector>
#include <array>
#include <iostream>
#include <fstream>
#include <random>
#include <filesystem>
#include <conio.h>
#include <mmsystem.h>
#pragma comment(linker, "\"/manifestdependency:type='win32' \
name = 'Microsoft.Windows.Common-Controls' version = '6.0.0.0' \
processorArchitecture = '*' publicKeyToken = '6595b64144ccf1df' language = '*'\"")
#define FPSLIMIT 60
#define WIN_MENU_LONAXIS 28 // 横长
#define WIN_MENU_HORAXIS 9  // 纵长
HANDLE hOutStd;
HWND hwnd;

class FrameRateController
{
public:
    FrameRateController()
    {
        resetStartTime();
    }
    double getFps()
    {
        QueryPerformanceFrequency((LARGE_INTEGER *)&time_union);
        timeFreq = (double)time_union.QuadPart;
        getCurrentTime();
        double timeDiff = (timeNow - startTime) / timeFreq;
        fps = 1000.0 / (timeDiff * 1000);
        return fps;
    }
    void printFps()
    {
        int i = 0;
    x:
        // sleepMicroseconds(100);
        double fps = getFps();
        for (; (int)fps > FPSLIMIT;)
        {
            i++;
            goto x;
        }
        std::cout << "\tFPS: " << fps << "  "
                  << "uselessCirPerFrame:" << i << " ";
        std::cout.flush();
    }
    void resetStartTime()
    {
        QueryPerformanceCounter(&time_union);
        startTime = time_union.QuadPart;
    }
    void sleepMicroseconds(DWORD microseconds)
    {
        TIMECAPS tc;
        timeGetDevCaps(&tc, sizeof(TIMECAPS));
        timeBeginPeriod(tc.wPeriodMin);
        Sleep(microseconds / 1000);
        timeEndPeriod(tc.wPeriodMin);
    }

private:
    LARGE_INTEGER time_union;
    double startTime;
    double timeFreq;
    double timeNow;
    double fps;
    void getCurrentTime()
    {
        QueryPerformanceCounter(&time_union);
        timeNow = time_union.QuadPart;
    }
};
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
    static void SetConTitle(const std::string &title)
    {
        SetConsoleTitleA(title.c_str());
    }
};
ConsoleStyle cstyle;
void readMapFromFile(std::vector<std::array<char, 64>> &map, const std::string &filename)
{
    std::ifstream file(filename);
    if (file.is_open())
    {
        std::string line;
        while (std::getline(file, line))
        {
            if (line.size() != 64)
            {
                std::cout << "Invalid line size: " << line.size() << std::endl;
                continue;
            }
            std::array<char, 64> row;
            for (size_t i = 0; i < 64; i++)
            {
                row[i] = line[i];
            }
            map.push_back(row);
        }
        file.close();
        std::cout << "Map read from file successfully.";
        std::cout.flush();
    }
    else
    {
        std::cout << "Failed to open file: " << filename;
        std::cout.flush();
    }
}
void writeMapToFile(const std::vector<std::array<char, 64>> &map, const std::string &filename)
{
    std::ofstream file(filename);
    if (file.is_open())
    {
        for (const auto &row : map)
        {
            for (const auto &element : row)
            {
                file << element;
            }
            file << std::endl;
        }
        file.close();
        std::cout << "Map written to file successfully." << std::endl;
    }
    else
    {
        std::cout << "Failed to open file: " << filename << std::endl;
    }
}
void SetColor(UINT uFore, UINT uBack)
{
    static HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(handle, uFore + uBack * 0x10);
}
void PrintMapByCol(const std::vector<std::array<char, 64>> *map)
{
    for (const auto &row : (*map))
    {
        static int index = 0;
        std::cout << "line:" << index << " ";
        for (const auto &element : row)
        {
            std::cout << element;
        }
        std::cout << std::endl;
        index++;
    }
}
void PrintMapByRow(const std::vector<std::array<char, 64>> *map)
{
    for (size_t col = 0; col < 64; col++)
    {
        for (size_t row = 0; row < map->size(); row++)
        {
            std::cout << (*map)[row][col];
        }
        std::cout << std::endl;
    }
}
void PrintMapByRange(const std::vector<std::array<char, 64>> &map, int rowIdx, int colIdx)
{
    int printWidth = 29;
    int printHeight = 48;

    int startRow = rowIdx - printHeight / 2;
    int startCol = colIdx - printWidth / 2;

    std::string message = "  ";

    for (int col = 0; col < printWidth; col++)
    {
        for (int row = 0; row < printHeight; row++)
        {
            int mapCol = startCol + col;
            int mapRow = startRow + row;

            // 中央索引坐标打印X
            if (mapRow == rowIdx && mapCol == colIdx)
            {
                std::cout << "☯ ";
            }
            else if (mapRow >= 0 && mapRow < map.size() && mapCol >= 0 && mapCol < 64)
            {
                // std::cout << map[mapRow][mapCol];
                switch (map[mapRow][mapCol])
                {
                case '0':
                    std::cout << "\033[30m\033[45m"
                              << "▒▒"
                              << "\033[0m";
                    break;
                case '1':
                    std::cout << "\033[37m\033[47m"
                              << "  "
                              << "\033[0m";
                    break;
                case '2':
                    std::cout << "\033[30m\033[42m"
                              << "XX"
                              << "\033[0m";
                    break;
                case '3':
                    std::cout << "\033[34m"
                              << "██"
                              << "\033[0m";
                    break;
                case '4':
                    std::cout << "▩ ";
                    break;
                case '5':
                    std::cout << "\033[30m\033[47m"
                              << "▓▓"
                              << "\033[0m";
                    break;
                case '6':
                    std::cout << "☐";
                    break;
                case '7':
                    std::cout << "☰";
                    break;
                case '8':
                    std::cout << "▓▓";
                    break;
                case '9':
                    std::cout << "▓▓";
                }
            }
            else
            {
                std::cout << "  ";
            }
        }
        if (col < printWidth - 1)
            std::cout << std::endl;
    }
}
void AppendMapToFile(const std::string &filename)
{
    std::ofstream file(filename, std::ios::app);
    if (file.is_open())
    {
        std::random_device rd;
        std::default_random_engine engine(rd());
        std::uniform_real_distribution<double> dist(0.0, 1.0);

        for (int i = 0; i < 1024; i++)
        {
            std::array<char, 64> row;
            for (size_t j = 0; j < 64; j++)
            {
                if (dist(engine) < static_cast<double>(j) / 64)
                {
                    row[j] = '0';
                }
                else
                {
                    row[j] = '1';
                }
            }

            for (const auto &element : row)
            {
                file << element;
            }
            file << std::endl;
        }

        file.close();
        std::cout << "Map appended to file successfully." << std::endl;
    }
    else
    {
        std::cout << "Failed to open file: " << filename << std::endl;
    }
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
void SetConsoleWindowPosition(int x, int y)
{
    HWND hwnd = GetConsoleWindow();
    RECT r;
    GetWindowRect(hwnd, &r);

    int width = r.right - r.left;
    int height = r.bottom - r.top;

    int screen_width = GetSystemMetrics(SM_CXSCREEN);
    int screen_height = GetSystemMetrics(SM_CYSCREEN);

    if (x == -1)
    {
        x = (screen_width - width) / 2;
    }
    if (y == -1)
    {
        y = (screen_height - height) / 2;
    }

    MoveWindow(hwnd, x, y, width, height, TRUE);
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
void InitTestEnv()
{
    // 关闭IO同步
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    system("mode con cols=96 lines=30");
    // 窗口置顶
    SetConsoleWindowPosition(-1, -1);
    SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_DRAWFRAME);
    cstyle.SetFont(L"Consolas", 18);
}
int CoreCircle(void)
{
    InitTestEnv();
    // 全局帧数计数器
    unsigned long long int frameCount = 0;
    FrameRateController FPS;

    // 创建地图
    std::vector<std::array<char, 64>> map;
    int rolindex = 1;
    int colindex = 1;
    char playerstate = 's';
    readMapFromFile(map, "map.txt");

    // 写一个循环,循环中非阻塞读取键盘输入,然后使用switch对awsd四个按键进行响应,即改变相应的地图索引值
    for (frameCount = 0; true; frameCount++)
    {
        FPS.resetStartTime();
        if (frameCount % 2 == 0)
        {
            if (GetAsyncKeyState('W') & 0x8000) // 检查W键是否被按下，并且地图数据中相应位置为1
            {
                if (map[rolindex][colindex - 1] == '1')
                {
                    colindex--;
                }
                playerstate = 'w';
            }
            if (GetAsyncKeyState('S') & 0x8000) // 检查S键是否被按下，并且地图数据中相应位置为1
            {
                if (map[rolindex][colindex + 1] == '1')
                {
                    colindex++;
                }
                playerstate = 's';
            }
            if (GetAsyncKeyState('A') & 0x8000) // 检查A键是否被按下，并且地图数据中相应位置为1
            {
                if (map[rolindex - 1][colindex] == '1')
                {
                    rolindex--;
                }
                playerstate = 'a';
            }
            if (GetAsyncKeyState('D') & 0x8000) // 检查D键是否被按下，并且地图数据中相应位置为1
            {
                if (map[rolindex + 1][colindex] == '1')
                {
                    rolindex++;
                }
                playerstate = 'd';
            }
            if (GetAsyncKeyState('J') & 0x8000) // 检查J键是否被按下
            {
                switch (playerstate)
                {
                case 'a':
                    map[rolindex - 1][colindex] = '1';
                    break;
                case 'd':
                    map[rolindex + 1][colindex] = '1';
                    break;
                case 'w':
                    map[rolindex][colindex - 1] = '1';
                    break;
                case 's':
                    map[rolindex][colindex + 1] = '1';
                    break;
                default:
                    break;
                }
            }
            if (GetAsyncKeyState('Q') & 0x8000) // 检查1键是否被按下
            {
                break;
            }
        }
        SetPosition(0, 1);
        PrintMapByRange(map, rolindex, colindex);
        std::cout.flush();
        SetPosition(40, 0);
        std::cout << "Frame: " << frameCount;
        std::cout.flush();
        FPS.printFps();
    }
    int num = map.size();
    std::cout << num << std::endl;
    writeMapToFile(map, "map.txt");
    system("pause");
    return 0;
}
void InitMainDrive(void)
{
    // 定义窗口句柄变量,包括窗口句柄,标准输出句柄等
    hwnd = GetConsoleWindow();
    hOutStd = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleWindowPosition(-1, -1);
    // 开启ANSI转义序列功能
    OpenANSIControlChar();
    cstyle.SetFont(L"新宋体", 22);
    cstyle.SetConTitle("BSIF | MAIN");

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
    std::cout << "\033[33m\033[1m\033[46m"
              << "\n---------BSIF 6.0.1---------"
              << "\033[36m\033[1m\033[40m"
              << "\n\n    \t  1. 测试\n    \t  2. 关于\n    \t  S. 设置\n    \t  Q. 退出"
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
            CoreCircle();
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