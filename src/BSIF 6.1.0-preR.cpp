// BASICINTERFACE 6Gen1 - 基本接口工程第六代
// Ver 6.1.0 - preRelease
// 2020.08.28 - 2020.09.17 - 2021.09.27 - 2023.05.23 - 2023.12.09

#include "BSIF.h"

class FrameRateController
{
public:
    int uselesscir = 0;
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
    double getFpsValue()
    {
        return fps;
    }
    void printFps()
    {
        uselesscir = 0;
    x:
        // sleepMicroseconds(100);
        double fps = getFps();
        for (; (int)fps > FPSLIMIT;)
        {
            uselesscir++;
            goto x;
        }
        std::cout << "\tFPS: " << fps << "  "
                  << "uselessCirPerFrame:" << uselesscir << " ";
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
class WindowDisplay
{
private:
    std::vector<std::string> window; // 存储窗口中的文本
    int windowWidth;                 // 窗口宽度
    int windowHeight;                // 窗口高度
    int windowX;                     // 内窗口起始X坐标
    int windowY;                     // 内窗口起始Y坐标
    int foregroundColor;             // 前景色
    int backgroundColor;             // 背景色
public:
    void init_Window_Str()
    {
        window.clear();
        // 初始化窗口，每行都为空字符串
        for (int i = 0; i < windowHeight; i++)
        {
            window.push_back("");
        }
    }
    void put_In_Text(int x, int y, const std::string &text)
    {
        int row = y;
        int col = x;
        for (const char &c : text)
        {
            if (col >= windowWidth)
            { // 如果到达窗口边界，换行
                col = 0;
                row++;
            }
            if (row >= windowWidth)
            {
                // 如果超出窗口高度，结束输出
                break;
            }
            window[row] += c;
            col++;
        }
    }

    void display_Window_Str()
    {
        int row = 0;
        SetColor(foregroundColor, backgroundColor);
        for (const std::string &line : window)
        {

            SetPosition(windowX, windowY + row);
            std::cout << line;
            std::cout.flush();
            row++;
        }
        SetColor(7, 0);
        init_Window_Str();
    }
    void cui_basic_fill(char ch)
    {
        COORD position{};
        position.X = windowX + 1;
        position.Y = windowY + 1;
        SetColor(foregroundColor, backgroundColor);
        for (int h = 1; h <= windowHeight; h++)
        {
            position.Y = windowY + h - 1;
            SetPosition(windowX, position.Y);
            for (int l = 0; l <= windowWidth - 1; l++)
                std::cout << ch;
            std::cout.flush();
        }
        SetColor(7, 0);
    }
    void init_new_window(int x, int y, int lenth, int height, int foreColor, int backColor)
    {
        windowWidth = lenth - 2;
        windowHeight = height - 2;
        windowX = x + 1;
        windowY = y + 1;
        foregroundColor = foreColor;
        backgroundColor = backColor;

        COORD position{};
        position.X = x;
        position.Y = y;

        SetColor(foregroundColor, backgroundColor);
        SetPosition(x + 1, y);
        for (int i = 1; i < lenth - 1; i++)
            std::cout << "━";
        std::cout.flush();
        SetPosition(x + 1, y + height - 1);
        for (int i = 1; i < lenth - 1; i++)
            std::cout << "━";
        std::cout.flush();

        SetPosition(x, y + 1);
        position.Y = y + 1;
        for (int n = 1; n <= height - 2; n++, position.Y++)
        {
            SetPosition(x, position.Y);
            std::cout << "┃";
            std::cout.flush();
        }

        SetPosition(x + lenth - 1, y + 1);
        position.Y = y + 1;
        for (int n = 1; n <= height - 2; n++, position.Y++)
        {
            SetPosition(x + lenth - 1, position.Y);
            std::cout << "┃";
            std::cout.flush();
        }

        SetPosition(x, y);
        std::cout << "┏";
        std::cout.flush();
        SetPosition(x + lenth - 1, y);
        std::cout << "┓";
        std::cout.flush();
        SetPosition(x, y + height - 1);
        std::cout << "┗";
        std::cout.flush();
        SetPosition(x + lenth - 1, y + height - 1);
        std::cout << "┛";
        SetColor(7, 0);
        std::cout.flush();
        init_Window_Str();
    }
    void init_noboundary_window(int x, int y, int lenth, int height)
    {
        COORD position{};
        position.X = x;
        position.Y = y;
        std::cout << "\033[46m";
        SetPosition(x, y);
        for (int i = 1; i <= lenth; i++)
            std::cout << " ";
        std::cout.flush();
        SetPosition(x, y + height - 1);
        for (int i = 1; i <= lenth; i++)
            std::cout << " ";
        std::cout.flush();

        SetPosition(x, y + 1);
        position.Y = y + 1;
        for (int n = 1; n <= height - 2; n++, position.Y++)
        {
            SetPosition(x, position.Y);
            std::cout << " ";
            std::cout.flush();
        }

        SetPosition(x + lenth - 1, y + 1);
        position.Y = y + 1;
        for (int n = 1; n <= height - 2; n++, position.Y++)
        {
            SetPosition(x + lenth - 1, position.Y);
            std::cout << " ";
            std::cout.flush();
        }
        std::cout << "\033[0m";
    }
};
class Entity
{
private:
    int id;
    std::string name;
    int hp;
    int sp;
    int x;
    int y;

public:
    Entity(int id, const std::string &name, int x, int y) : id(id), name(name), hp(100), sp(100), x(x), y(y) {}
    int getId() const
    {
        return id;
    }
    std::string getName() const
    {
        return name;
    }
    int getHp() const
    {
        return hp;
    }
    int getSp() const
    {
        return sp;
    }
    int getX() const
    {
        return x;
    }
    int getY() const
    {
        return y;
    }
    void setHp(int newHp)
    {
        hp = newHp;
    }
    void setSp(int newSp)
    {
        sp = newSp;
    }
};
class EntityManager
{
private:
    std::vector<Entity> entities;

public:
    void addEntity(int id, const std::string &name, int x, int y)
    {
        Entity entity(id, name, x, y);
        entities.push_back(entity);
    }

    Entity *getEntityByName(const std::string &name)
    {
        for (auto &entity : entities)
        {
            if (entity.getName() == name)
            {
                return &entity;
            }
        }
        return nullptr;
    }
};
struct VirtualObject
{
    int x;
    int y;
    int lx;
    int ly;
    int dir;
    int att;
    int speed;
    int len;
    int exist;
    int attribute;
};
class VirtualObjectManager
{
private:
    static const int MAX_OBJECTS = 100; // 最大虚拟体数量
    VirtualObject objects[MAX_OBJECTS]; // 虚拟体数组
public:
    VirtualObjectManager()
    {
        for (int i = 0; i < MAX_OBJECTS; i++)
        {
            objects[i].exist = 0; // 初始化所有节点的 exist 为 0
        }
    }
    // 添加新的虚拟体
    void addObject(int x, int y)
    {
        int index = findEmptyIndex(); // 查找空闲节点的索引
        if (index != -1)
        {
            VirtualObject &object = objects[index];
            object.x = x;
            object.y = y;
            object.exist = 1; // 设置 exist 为 1，表示有数据
        }
    }
    // 销毁虚拟体
    void destroyObject(int index)
    {
        if (index >= 0 && index < MAX_OBJECTS)
        {
            objects[index].exist = 0; // 设置 exist 为 0，表示无数据
        }
    }

    // 根据索引获取虚拟体
    VirtualObject *getObjectByIndex(int index)
    {
        if (index >= 0 && index < MAX_OBJECTS && objects[index].exist == 1)
        {
            return &objects[index];
        }
        return nullptr;
    }
    // 更新虚拟体的字段值
    void updateObjectField(int index, const std::string &fieldName, int newValue)
    {
        if (index >= 0 && index < MAX_OBJECTS && objects[index].exist == 1)
        {
            VirtualObject &object = objects[index];
            if (fieldName == "x")
            {
                object.x = newValue;
            }
            else if (fieldName == "y")
            {
                object.y = newValue;
            }
            else if (fieldName == "lx")
            {
                object.lx = newValue;
            }
            else if (fieldName == "ly")
            {
                object.ly = newValue;
            }
            else if (fieldName == "dir")
            {
                object.dir = newValue;
            }
            else if (fieldName == "att")
            {
                object.att = newValue;
            }
            else if (fieldName == "speed")
            {
                object.speed = newValue;
            }
            else if (fieldName == "len")
            {
                object.len = newValue;
            }
            else if (fieldName == "exist")
            {
                object.exist = newValue;
            }
            else if (fieldName == "attribute")
            {
                object.attribute = newValue;
            }
            else
            {
                std::cout << "Invalid field name." << std::endl;
            }
        }
    }
private:
    // 查找空闲节点的索引
    int findEmptyIndex()
    {
        for (int i = 0; i < MAX_OBJECTS; i++)
        {
            if (objects[i].exist == 0)
            {
                return i;
            }
        }
        return -1; // 没有空闲节点
    }
};
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
    std::cout.flush();
    static HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(handle, uFore + uBack * 0x10);
}
COORD GetColor(void)
{
    CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo;
    GetConsoleScreenBufferInfo(hOutStd, &screenBufferInfo);

    WORD attributes = screenBufferInfo.wAttributes;
    WORD foregroundColor = attributes & 0x0F;
    WORD backgroundColor = (attributes & 0xF0) >> 4;

    COORD color;
    color.X = foregroundColor;
    color.Y = backgroundColor;
    return color;
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

    for (int col = 0; col < printWidth; col++)
    {
        for (int row = 0; row < printHeight; row++)
        {
            int mapCol = startCol + col;
            int mapRow = startRow + row;

            // 中央索引坐标打印X
            if (mapRow == rowIdx && mapCol == colIdx)
            {
                std::cout << "\033[30m\033[47m"
                          << "☯ "
                          << "\033[0m";
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
                              << "╬╬"
                              << "\033[0m";
                    break;
                case '3':
                    std::cout << "\033[34m"
                              << "██"
                              << "\033[0m";
                    break;
                case '4':
                    std::cout << "📶";
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
        {
            std::cout << std::endl;
        }
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
    system("cls");
    // 关闭IO同步
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    system("mode con cols=126 lines=30");

    SetConsoleWindowPosition(-1, -1);
    // 窗口置顶
    SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_DRAWFRAME);
    cstyle.SetFont(L"Consolas", 18);
}
int CoreCircle(void)
{
    InitTestEnv();
    // 全局帧数计数器
    unsigned long long int frameCount = 0;
    FrameRateController FPS;

    // 性能日志
    std::ofstream fpsdata;
    fpsdata.open("fpsdata.txt", std::ios::out | std::ios::trunc);

    // 创建地图
    std::vector<std::array<char, 64>> map;
    int rolindex = 1;
    int colindex = 1;
    char playerstate = 's';
    readMapFromFile(map, "map.txt");
    WindowDisplay infowindow;

    infowindow.init_new_window(96, 1, 30, 29, 0x0, 0xb);
    infowindow.cui_basic_fill(' ');

    // 核心循环,循环中非阻塞读取键盘输入,然后使用switch对awsd四个按键进行响应,即改变相应的地图索引值
    for (frameCount = 0; true; frameCount++)
    {
        FPS.resetStartTime();
        infowindow.put_In_Text(0, 0, "|frame:" + std::to_string(frameCount) + " ");
        infowindow.put_In_Text(0, 1, "|FPS:" + std::to_string(FPS.getFpsValue()) + " ");
        infowindow.put_In_Text(0, 3, "|playerstate:" + std::to_string(playerstate) + " ");
        infowindow.put_In_Text(0, 4, "|rolindex:" + std::to_string(rolindex) + " ");
        infowindow.put_In_Text(0, 5, "|colindex:" + std::to_string(colindex) + " ");
        infowindow.display_Window_Str();
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
        fpsdata << frameCount << "|" << FPS.getFps() << std::endl;
    }

    int num = map.size();
    std::cout << num << std::endl;

    writeMapToFile(map, "map.txt");
    fpsdata.close();
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

    // SMALL_RECT windowSize = {0, 0, WIN_MENU_LONAXIS - 1, WIN_MENU_HORAXIS - 2};
    // SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, &windowSize);
    // COORD bufferSize = {WIN_MENU_LONAXIS, WIN_MENU_HORAXIS - 1};
    // SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), bufferSize);

    // 窗口置顶
    SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_DRAWFRAME);

    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
}
int main(void)
{
    InitMainDrive();
printMenu_:
    for (int index = 1; index < 13; index++)
    {
        SetPosition(1, index);
        for (int indexx = 1; indexx < WIN_MENU_LONAXIS - 1; indexx++)
        {
            std::cout << "\033[40m"
                      << " ";
        }
        std::cout.flush();
    }
    for (int index = 14; index < WIN_MENU_HORAXIS - 1; index++)
    {
        SetPosition(1, index);
        for (int indexx = 1; indexx < WIN_MENU_LONAXIS - 1; indexx++)
        {
            std::cout << "\033[37\033[40m"
                      << " ";
        }
        std::cout.flush();
    }

    std::cout << "\033[0m";
    SetPosition(0, 0);
    std::string logotxt = R"(
           ___           ___                       ___     
          /\  \         /\  \          ___        /\  \
         /  \  \       /  \  \        /\  \      /  \  \
        / /\ \  \     / /\ \  \       \ \  \    / /\ \  \
       /  \ \ \__\   _\ \ \ \  \      /  \__\  /  \ \ \  \
      / /\ \ \ |__| /\ \ \ \ \__\  __/ /\/__/ / /\ \ \ \__\
      \ \ \ \/ /  / \ \ \ \ \/__/ /\/ /  /    \/__\ \ \/__/
       \ \ \  /  /   \ \ \ \__\   \  /__/          \ \__\
        \ \/ /  /     \ \/ /  /    \ \__\           \/__/  
         \  /__/       \  /  /      \/__/                  
          ~~            \/__/                              
     )";
    std::cout << "\033[1;32m\033[1m\033[5m\033[40m" << logotxt << "\033[0m";
    std::cout.flush();

    SetPosition(8, 12);
    std::cout << "\033[33m\033[1m\033[46m"
              << "\n-------------------" << VERSION_TXT << "---------------------"
              << "\033[36m\033[1m\033[40m"
              << "\n\n    \t\t\t   1. 测试\n\n    \t\t\t   2. 关于\n\n    \t\t\t   S. 设置\n\n    \t\t\t   Q. 退出"
              << "\033[0m";
    std::cout.flush();

    SetPosition(0, 0);
    for (int i = 0; i <= WIN_MENU_LONAXIS - 1; i++)
        std::cout << "\033[32m" << '=' << "\033[0m";
    std::cout.flush();

    SetPosition(0, WIN_MENU_HORAXIS - 2);
    for (int i = 0; i <= WIN_MENU_LONAXIS - 1; i++)
        std::cout << "\033[32m" << '=' << "\033[0m";
    std::cout.flush();

    for (int i = 0; i <= WIN_MENU_HORAXIS - 2; i++)
    {
        SetPosition(0, i);
        std::cout << "|";
        std::cout.flush();
    }
    for (int i = 0; i <= WIN_MENU_HORAXIS - 2; i++)
    {
        SetPosition(WIN_MENU_LONAXIS - 1, i);
        std::cout << "|";
        std::cout.flush();
    }

    while (true)
    {
        if (GetAsyncKeyState('1') & 0x8000) // 检查S键是否被按下，并且地图数据中相应位置为1
        {
            CoreCircle();
        }
        if (GetAsyncKeyState('2') & 0x8000) // 检查S键是否被按下，并且地图数据中相应位置为1
        {
            MessageBoxExA(GetConsoleWindow(), "Copyright 2023 Zhou Xu.All rights reserved.", (VERSION_TXT), MB_OK | MB_ICONASTERISK | MB_TOPMOST, 0);
            continue;
        }
        if (GetAsyncKeyState('S') & 0x8000) // 检查S键是否被按下，并且地图数据
        {
            WindowDisplay setWindow;

            setWindow.init_new_window(10, 10, 35, 8, 0x0, 0xb);
            setWindow.cui_basic_fill(' ');
            setWindow.put_In_Text(0, 0, "测试");
            setWindow.display_Window_Str();
            continue;
        }
        if (GetAsyncKeyState('Q') & 0x8000) // 检查S键是否被按下
        {
            break;
        }
        if (is_esc_permitted == true && (GetAsyncKeyState(VK_ESCAPE) & 0x8000)) // 检查ESC键是否被按下
        {
            is_esc_permitted = false;
            system("cls");
            SetPosition(0, 0);
            goto printMenu_;
        }
        if (!(GetAsyncKeyState(VK_ESCAPE) & 0x8000)) // 检查ESC键是否被松开
        {
            is_esc_permitted = true;
        }
    }
    std::cin.get();
    return 0;
}