// 测试Windows控制台输出特殊字符的程序
// 使用Windows API实现光标的定位等操作
#include <windows.h>
#include <vector>
#include <array>
#include <iostream>
#include <fstream>
#include <random>
#include <filesystem>
#include <conio.h>
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
        std::cout << "Map read from file successfully." << std::endl;
    }
    else
    {
        std::cout << "Failed to open file: " << filename << std::endl;
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
    int printWidth = 41;
    int printHeight = 19;

    int startRow = rowIdx - printHeight / 2;
    int startCol = colIdx - printWidth / 2;

    for (int col = 0; col < printHeight; col++)
    {
        for (int row = 0; row < printWidth; row++)
        {
            int mapCol = startCol + col;
            int mapRow = startRow + row;

            // 中央索引坐标打印X
            if (mapRow == rowIdx && mapCol == colIdx)
            {
                std::cout << "X";
            }
            else if (mapRow >= 0 && mapRow < map.size() && mapCol >= 0 && mapCol < 64)
            {
                std::cout << map[mapRow][mapCol];
            }
            else
            {
                std::cout << " ";
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
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cur_info);
    cur_info.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cur_info);

    // 创建地图
    std::vector<std::array<char, 64>> map;
    int rolindex = 0;
    int colindex = 0;
    readMapFromFile(map, "map.txt");

    // 写一个循环,循环中非阻塞读取键盘输入,然后使用switch对awsd四个按键进行响应,即改变相应的地图索引值
    while (true)
    {
        Sleep(1);
        if (GetAsyncKeyState('W') & 0x8000) // 检查W键是否被按下
        {
            colindex--;
        }
        if (GetAsyncKeyState('S') & 0x8000) // 检查S键是否被按下
        {
            colindex++;
        }
        if (GetAsyncKeyState('A') & 0x8000) // 检查A键是否被按下
        {
            rolindex--;
        }
        if (GetAsyncKeyState('D') & 0x8000) // 检查D键是否被按下
        {
            rolindex++;
        }
        SetPosition(0, 1);
        printMapByRange(map, rolindex, colindex);
    }
    int num = map.size();
    std::cout << num << std::endl;
    writeMapToFile(map, "map.txt");
    system("pause");
    return 0;
}