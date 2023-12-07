// BSIF 6Gen1 - 基本接口工程第六代
// Ver 6.1.0 - pre
#include <windows.h>
#include <vector>
#include <array>
#include <iostream>
#include <fstream>
#include <random>
#include <filesystem>
#include <conio.h>
#include <mmsystem.h>
#define FPSLIMIT 120
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
        //sleepMicroseconds(100);
        double fps = getFps();
        for (; (int)fps > FPSLIMIT;)
        {
            i++;
            goto x;
        }
        std::cout << "\tFPS: " << fps << "  "
                  << "uselessCirPerFrame:" << i << "    ";
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
std::uintmax_t getFileSize(const std::string &filename)
{
    std::filesystem::path filePath(filename);
    return std::filesystem::file_size(filePath);
}
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
void printMapByCol(const std::vector<std::array<char, 64>> *map)
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
void printMapByRow(const std::vector<std::array<char, 64>> *map)
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
void printMapByRange(const std::vector<std::array<char, 64>> &map, int rowIdx, int colIdx)
{
    int printWidth = 25;
    int printHeight = 48;

    int startRow = rowIdx - printHeight / 2;
    int startCol = colIdx - printWidth / 2;

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
                    std::cout << "██";
                    break;
                case '1':
                    std::cout << "  ";
                    break;
                case '2':
                    std::cout << "▒▒";
                    break;
                case '3':
                    std::cout << "▓▓";
                    break;
                case '4':
                    std::cout << "▩ ";
                    break;
                case '5':
                    std::cout << "☰";
                    break;
                case '6':
                    std::cout << "☐";
                    break;
                case '7':
                    std::cout << "▓▓";
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
        std::cout << std::endl;
    }
}
void appendMapToFile(const std::string &filename)
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

int main(void)
{
    // 全局帧数计数器
    unsigned long long int frameCount = 0;
    FrameRateController FPS;

    // 关闭IO同步
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    system("mode con cols=96 lines=27");

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
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cur_info);
    cur_info.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cur_info);

    // 创建地图
    std::vector<std::array<char, 64>> map;
    int rolindex = 1;
    int colindex = 1;
    readMapFromFile(map, "map.txt");

    // 写一个循环,循环中非阻塞读取键盘输入,然后使用switch对awsd四个按键进行响应,即改变相应的地图索引值
    for (frameCount = 0; true; frameCount++)
    {
        FPS.resetStartTime();
        if (frameCount % 3 == 0)
        {
            if ((GetAsyncKeyState('W') & 0x8000) && map[rolindex][colindex - 1] == '1') // 检查W键是否被按下,并且地图数据中相应位置为1
            {
                colindex--;
            }
            if (GetAsyncKeyState('S') & 0x8000 && map[rolindex][colindex + 1] == '1') // 检查S键是否被按下
            {
                colindex++;
            }
            if (GetAsyncKeyState('A') & 0x8000 && map[rolindex - 1][colindex] == '1') // 检查A键是否被按下
            {
                rolindex--;
            }
            if (GetAsyncKeyState('D') & 0x8000 && map[rolindex + 1][colindex] == '1') // 检查D键是否被按下
            {
                rolindex++;
            }
        }
        SetPosition(0, 1);
        printMapByRange(map, rolindex, colindex);
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