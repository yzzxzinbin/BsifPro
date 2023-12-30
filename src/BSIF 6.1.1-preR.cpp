// BASICINTERFACE 6Gen1 - 基本接口工程第六代
// Ver 6.1.1 - preRelease
// 2020.08.28 - 2020.09.17 - 2021.09.27 - 2023.05.23 - 2023.12.09

#include "BSIF.h"

class FrameRateController // 帧率时控类
{
public:
    FrameRateController() // 构造函数:初始化高精度计时器
    {
        timeGetDevCaps(&tc, sizeof(TIMECAPS));
        timeBeginPeriod(tc.wPeriodMin);
        resetStartTime();
    }
    double getFps() // 获取帧率(计算)
    {
        QueryPerformanceFrequency((LARGE_INTEGER *)&time_union);
        timeFreq = (double)time_union.QuadPart;
        getCurrentTime();
        double timeDiff = (timeNow - startTime) / timeFreq;
        fps = 1000.0 / (timeDiff * 1000);
        return fps;
    }
    double getFpsValue() // 获取帧率(数值)
    {
        return fps;
    }
    void printFps() // 输出帧率
    {
        double fps = getFpsLimited();
        std::cout << "\tFPS: " << fps << "  "
                  << "uselessCirPerFrame:" << uselesscir << " ";
    }
    double getFpsLimited(void) // 限制帧率
    {
        uselesscir = 0;
    z:
        fps = getFps();
        for (; (int)fps > SETITEM_limitedFps;)
        {
            uselesscir++;
            sleepMicroseconds(1000);
            goto z;
        }
        return fps;
    }
    void resetStartTime() // 初始化高精度计时器
    {
        QueryPerformanceCounter(&time_union);
        startTime = time_union.QuadPart;
    }
    void sleepMicroseconds(DWORD microseconds)
    {
        Sleep(microseconds / 1000);
    }

private:
    LARGE_INTEGER time_union; // 晶振级高精度计时器
    TIMECAPS tc;              // 毫秒级高精度计时器
    double startTime;         // 晶振起始值
    double timeFreq;          // 晶振频率
    double timeNow;           // 晶振当前值
    double fps;               // 帧率

public:
    int uselesscir = 0;
    void getCurrentTime() // 更新晶振当前值
    {
        QueryPerformanceCounter(&time_union);
        timeNow = time_union.QuadPart;
    }

public:
    int getUselesscir() // 获取无用循环数
    {
        return uselesscir;
    }

    ~FrameRateController() // 析构函数:还原多媒体计时器精度
    {
        timeEndPeriod(tc.wPeriodMin);
    }
};
class ConsoleStyle // 控制台样式
{
public:
    static void SetFont(const wchar_t *fontName, int fontSize)
    { // 设置控制台字体
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
    { // 设置控制台颜色
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFOEX screenBufferInfo;
        screenBufferInfo.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
        GetConsoleScreenBufferInfoEx(hConsole, &screenBufferInfo);
        screenBufferInfo.ColorTable[colorIndex] = color;
        SetConsoleScreenBufferInfoEx(hConsole, &screenBufferInfo);
    }
    static void SetConTitle(const std::string &title)
    { // 设置控制台标题
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
    { // 初始化窗口，每行都为空字符串
        window.clear();

        for (int i = 0; i < windowHeight; i++)
        {
            window.push_back("");
        }
    }
    void put_In_Text(int x, int y, const std::string &text)
    { // 将文本放入窗口,本函数负责换行和数据写入
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
        while (col < windowWidth)
        {
            window[row] += ' '; // 使用空格填充剩余列数
            col++;
        }
    }

    void display_Window_Str()
    { // 将窗口中的文本输出到控制台
        int row = 0;
        SetColor(foregroundColor, backgroundColor);
        for (const std::string &line : window)
        {

            std::cout << "\033[" << windowY + row + 1 << ";" << windowX + 1 << "H";
            // SetPosition(windowX, windowY + row);
            std::cout << line;
            // std::cout.flush();
            row++;
        }

        SetColor(7, 0);
        init_Window_Str();
    }
    void cui_Basic_Fill(char ch)
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
    void init_New_Window(int x, int y, int lenth, int height, int foreColor, int backColor)
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
    void init_Nobound_Window(int x, int y, int lenth, int height)
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
    char status;

public:
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
    char getStatus() const
    {
        return status;
    }
    void setHp(int newHp)
    {
        hp = newHp;
    }
    void setSp(int newSp)
    {
        sp = newSp;
    }
    void setName(const std::string &newName)
    {
        name = newName;
    }
    void setX(int newX)
    {
        if (newX == -2)
        {
            x--;
            return;
        }
        else if (newX == -1)
        {
            x++;
            return;
        }
        x = newX;
    }
    void setY(int newY)
    {
        if (newY == -2)
        {
            y--;
            return;
        }
        else if (newY == -1)
        {
            y++;
            return;
        }
        y = newY;
    }
    void setId(int newId)
    {
        id = newId;
    }
    void setStatus(char newStatus)
    {
        status = newStatus;
    }
};
class EntityManager
{
public:
    std::vector<Entity> entities;

public:
    void addEntity(const std::string &name, int id, int x, int y, int hp, int sp, char status)
    {
        // Entity entity(id, name, x, y);
        // 使用对应函数初始化值
        Entity entity;
        entity.setId(id);
        entity.setName(name);
        entity.setX(x);
        entity.setY(y);
        entity.setHp(hp);
        entity.setSp(sp);
        entity.setStatus(status);
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

public:
    // 从文件中读取实体数据
    bool readEntitiesFromFile(const std::string &filename)
    {
        entities.clear();
        std::ifstream file(filename);
        if (!file.is_open())
        {
            loginfo << FormatTime(time(nullptr)) << "Failed to open file: " << filename << std::endl;
            return false;
        }

        std::string line;
        while (std::getline(file, line))
        {
            std::vector<std::string> tokens = split(line, ',');
            if (tokens.size() == 7)
            {
                Entity entity;
                entity.setName(tokens[0]);
                entity.setId(std::stoi(tokens[1]));
                entity.setHp(std::stoi(tokens[2]));
                entity.setSp(std::stoi(tokens[3]));
                entity.setX(std::stoi(tokens[4]));
                entity.setY(std::stoi(tokens[5]));
                entity.setStatus(tokens[6][0]);
                entities.push_back(entity);
            }
            else
            {
                std::cout << "Invalid line: " << line << std::endl;
            }
        }

        file.close();
        return true;
    }

    // 将实体数据写入文件
    bool writeEntitiesToFile(const std::string &filename)
    {
        std::ofstream file;
        file.open(filename, std::ios::trunc);
        if (!file.is_open())
        {
            loginfo << FormatTime(time(nullptr)) << "Failed to open file: " << filename << std::endl;
            return false;
        }

        for (const Entity &entity : entities)
        {
            file << entity.getName() << "," << entity.getId() << ","
                 << entity.getHp() << "," << entity.getSp() << "," << entity.getX() << "," << entity.getY() << "," << entity.getStatus() << std::endl;
        }

        file.close();
        return true;
    }

private:
    std::vector<std::string> split(const std::string &str, char delimiter)
    {
        std::vector<std::string> tokens;
        std::string token;
        std::istringstream tokenStream(str);
        while (std::getline(tokenStream, token, delimiter))
        {
            tokens.push_back(token);
        }
        return tokens;
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
public:
    static const int MAX_OBJECTS = 6000; // 最大虚拟体数量
    VirtualObject objects[MAX_OBJECTS];  // 虚拟体数组
    int virUsedNum = 0;                  // 已使用的虚拟体数量,0表示objects为空
public:
    VirtualObjectManager()
    {
        initVirtualObjectManager();
    }
    void initVirtualObjectManager()
    {
        for (int i = 0; i < MAX_OBJECTS; i++)
        {
            objects[i].exist = 0; // 初始化所有节点的 exist 为 0
            virUsedNum = 0;       // 虚拟体数量初始化为 0
        }
    }
    // 添加新的虚拟体
    void addObject(int x, int y, int lx, int ly, int dir, int att, int speed, int len, int attribute)
    {

        int index = findEmptyIndex(); // 查找空闲节点的索引
        if (index != -1)
        {
            VirtualObject &object = objects[index];
            object.x = x;
            object.y = y;
            object.lx = lx;
            object.ly = ly;
            object.dir = dir;
            object.att = att;
            object.speed = speed;
            object.len = len;
            object.attribute = attribute;
            object.exist = 1; // 设置 exist 为 1，表示有数据
            virUsedNum++;     // 虚拟体数量加 1
        }
    }
    // 销毁虚拟体
    void destroyObject(int index)
    {
        if (index >= 0 && index < MAX_OBJECTS)
        {
            objects[index].exist = 0; // 设置 exist 为 0，表示无数据
            virUsedNum--;             // 虚拟体数量减 1
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
        exit(0); // 没有空闲节点
    }

public:
    // 从文件中读取虚拟体数据
    bool readVirtualObjectsFromFile(const std::string &filename)
    {
        initVirtualObjectManager();

        std::ifstream file(filename);

        if (!file.is_open())
        {
            loginfo << FormatTime(time(nullptr)) << "Failed to open file: " << filename << std::endl;
            return false;
        }

        std::string line;
        while (std::getline(file, line))
        {
            std::vector<std::string> fields = split(line, ',');
            if (fields.size() != 10)
            {
                loginfo << FormatTime(time(nullptr)) << "Invalid data format in file: " << filename << std::endl;
                return false;
            }

            VirtualObject &object = objects[virUsedNum];
            object.x = std::stoi(fields[0]);
            object.y = std::stoi(fields[1]);
            object.lx = std::stoi(fields[2]);
            object.ly = std::stoi(fields[3]);
            object.dir = std::stoi(fields[4]);
            object.att = std::stoi(fields[5]);
            object.speed = std::stoi(fields[6]);
            object.len = std::stoi(fields[7]);
            object.exist = std::stoi(fields[8]);
            object.attribute = std::stoi(fields[9]);
            if (object.exist == 1)
                virUsedNum++;
            if (virUsedNum >= MAX_OBJECTS)
            {
                break;
            }
        }

        file.close();
        return true;
    }

    // 将虚拟体数据写入文件
    bool writeVirtualObjectsToFile(const std::string &filename)
    {
        std::ofstream file(filename);
        if (!file.is_open())
        {
            loginfo << FormatTime(time(nullptr)) << "Failed to open file: " << filename << std::endl;
            return false;
        }

        for (int i = 0; i < MAX_OBJECTS; i++)
        {
            if (objects[i].exist == 1)
            {
                const VirtualObject &object = objects[i];
                file << object.x << "," << object.y << ","
                     << object.lx << "," << object.ly << ","
                     << object.dir << "," << object.att << ","
                     << object.speed << "," << object.len << ","
                     << object.exist << "," << object.attribute << std::endl;
            }
        }

        file.close();
        return true;
    }

private:
    std::vector<std::string> split(const std::string &str, char delimiter)
    {
        std::vector<std::string> tokens;
        std::string token;
        std::istringstream tokenStream(str);
        while (std::getline(tokenStream, token, delimiter))
        {
            tokens.push_back(token);
        }
        return tokens;
    }
};
EntityManager em;
VirtualObjectManager vom;
std::string FormatTime(time_t time)
{
    // 将时间转换为结构体
    struct tm *timeinfo;
    timeinfo = localtime(&time);

    // 使用stringstream来格式化时间
    std::stringstream ss;
    ss << (timeinfo->tm_year + 1900) << "."; // 年份需要加上1900
    ss << (timeinfo->tm_mon + 1) << ".";
    ss << timeinfo->tm_mday << " ";
    ss << timeinfo->tm_hour << ":";
    ss << timeinfo->tm_min << ":";
    ss << timeinfo->tm_sec;
    ss << "  |";

    // 返回格式化后的时间字符串
    return ss.str();
}
bool IsNumber(const std::string &str)
{
    try
    {
        std::stoi(str); // 尝试将字符串转换为整数
        return true;    // 转换成功，说明是数字
    }
    catch (const std::exception &e)
    {
        return false; // 转换失败，说明不是数字
    }
}
void ReadMapFromFile(std::vector<std::vector<char>> &map, const std::string &filename)
{
    std::ifstream file(filename);
    if (file.is_open())
    {
        std::string line;
        while (std::getline(file, line))
        {
            std::vector<char> row;
            for (size_t i = 0; i < line.size(); i++)
            {
                // if (line[i] == '0')
                // {
                //     line[i] = '1';
                // }
                // else if (line[i] == '1')
                // {
                //     line[i] = '0';
                // }
                row.push_back(line[i]);
            }
            map.push_back(row);
        }
        file.close();
        loginfo << FormatTime(time(nullptr)) << "Map read from file successfully." << std::endl;
    }
    else
    {
        loginfo << FormatTime(time(nullptr)) << "Failed to open file: " << filename;
    }
}

void WriteMapToFile(const std::vector<std::vector<char>> &map, const std::string &filename)
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
        loginfo << FormatTime(time(nullptr)) << "Map written to file successfully." << std::endl;
    }
    else
    {
        loginfo << FormatTime(time(nullptr)) << "Failed to open file: " << filename << std::endl;
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
void PrintMapByCol(const std::vector<std::vector<char>> *map)
{
    for (const auto &row : (*map))
    {
        for (const auto &element : row)
        {
            std::cout << element;
        }
        std::cout << std::endl;
    }
}
void PrintMapByRow(const std::vector<std::vector<char>> *map)
{
    for (int col = 0; col < map->at(0).size(); col++)
    {
        for (int row = 0; row < map->size(); row++)
        {
            for (const Entity &entity : em.entities)
            {
                if (entity.getName() == "player1" && entity.getY() == col && entity.getX() == row)
                {
                    std::cout << "\033[30m\033[47m"
                              << "☯ "
                              << "\033[0m";
                    goto nomapout;
                }
                else if (entity.getName() == "player2" && entity.getY() == col && entity.getX() == row)
                {
                    std::cout << "\033[30m\033[43m"
                              << "☯ "
                              << "\033[0m";
                    goto nomapout;
                }
            }
            {
                int virIndex = 0;
                int virUsedNumFixed = vom.virUsedNum;
                for (const VirtualObject &virobj : vom.objects)
                {
                    if (virIndex == virUsedNumFixed)
                    {
                        break;
                    }
                    if (virobj.exist == 1)
                    {
                        virIndex++;
                        if ((virobj.x == row) && (virobj.y == col))
                        {
                            switch (virobj.attribute)
                            {
                            case 0:
                                std::cout << "\033[30m\033[43m"
                                          << "Ⓘ "
                                          << "\033[0m";
                                goto nomapout;
                                break;
                            case 1:
                                std::cout << "\033[31m\033[41m"
                                          << "  "
                                          << "\033[0m";
                                goto nomapout;
                                break;
                            default:
                                break;
                            }
                        }
                    }
                }
            }
            switch ((*map)[row][col])
            {
            case '0':
                std::cout << "\033[37m\033[47m"
                          << "  "
                          << "\033[0m";
                break;
            case '1':
                std::cout << "\033[35m\033[45m"
                          << "▒▒"
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
            default:
                break;
            }
        nomapout:;
        }
        if (col != map->at(0).size() - 1)
            std::cout << "\e[1E";
    }
}
void PrintMapByRange(const std::vector<std::vector<char>> &map, int rowIdx, int colIdx)
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
                    std::cout << "\033[30m\033[47m"
                              << "  "
                              << "\033[0m";
                    break;
                case '1':
                    std::cout << "\033[37m\033[45m"
                              << "▒▒"
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
        loginfo << FormatTime(time(nullptr)) << "Failed to open file: " << filename << std::endl;
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
std::vector<std::string> split(const std::string &str, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);
    while (std::getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}
int DetectMap(const std::vector<std::vector<char>> &map, int x, int y, char status, char symbol)
{

    while (true)
    {
        switch (status)
        {
        case 'w':
            y--;
            if (map[x][y] != symbol)
            {
                return y;
            }
            break;
        case 's':
            y++;
            if (map[x][y] != symbol)
            {
                return y;
            }
            break;
        case 'a':
            x--;
            if (map[x][y] != symbol)
            {
                return x;
            }
            break;
        case 'd':
            x++;
            if (map[x][y] != symbol)
            {
                return x;
            }
            break;
        default:
            exit(0);
        }
    }
}
void InitTestEnv()
{
    system("cls");
    // 关闭IO同步
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    system("mode con cols=126 lines=30");

    // 禁用窗口最大化和最小化和动态调整窗口大小
    SetWindowLongPtrA(GetConsoleWindow(), GWL_STYLE, GetWindowLongPtrA(GetConsoleWindow(), GWL_STYLE) & ~WS_SIZEBOX & ~WS_MAXIMIZEBOX & ~WS_MINIMIZEBOX);
    SetConsoleWindowPosition(-1, -1);

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
    std::vector<std::vector<char>> map;
    ReadMapFromFile(map, "areamap.txt");
    // 读取虚拟体和实体数据

    em.readEntitiesFromFile("entities.txt");
    vom.readVirtualObjectsFromFile("virtualobjects.txt");
    int rolindex = 1;
    int colindex = 1;
    char playerstate = 's';

    WindowDisplay infowindow;

    infowindow.init_New_Window(96, 1, 30, 29, 0x0, 0xb);
    infowindow.cui_Basic_Fill(' ');

    // 核心循环,循环中非阻塞读取键盘输入,然后使用switch对awsd四个按键进行响应,即改变相应的地图索引值
    for (frameCount = 0; true; frameCount++)
    {
        FPS.resetStartTime();
        if (frameCount % 10 == 0)
        {
            infowindow.put_In_Text(0, 0, "|frame:" + std::to_string(frameCount));
            infowindow.put_In_Text(0, 1, "|FPS:" + std::to_string(FPS.getFpsValue()));
            infowindow.put_In_Text(0, 2, "|uselesscir:" + std::to_string(FPS.getUselesscir()));
            infowindow.put_In_Text(0, 3, "|player1state:" + std::to_string(em.entities[0].getStatus()));
            infowindow.put_In_Text(0, 4, "|rolindex:" + std::to_string(rolindex));
            infowindow.put_In_Text(0, 5, "|colindex:" + std::to_string(colindex));
            infowindow.put_In_Text(0, 6, "|entities:" + std::to_string(em.entities.size()));
            infowindow.put_In_Text(0, 7, "|en1x:" + std::to_string(em.entities[0].getX()));
            infowindow.put_In_Text(0, 8, "|en1y:" + std::to_string(em.entities[0].getY()));
            infowindow.put_In_Text(0, 9, "|virUsedNum:" + std::to_string(vom.virUsedNum));
            infowindow.put_In_Text(0, 10, "|en1hp:" + std::to_string(em.entities[0].getHp()));
        }
        infowindow.display_Window_Str();

        if (frameCount % 2 == 0)
        {
            if (GetAsyncKeyState('W') & 0x8000) // 检查W键是否被按下，并且地图数据中相应位置为1
            {
                if ((map[(em.entities[0].getX())][em.entities[0].getY() - 1] == '0')
                    //&& (em.entities[0].getStatus() == 'w')
                )
                {
                    em.entities[0].setY(-2);
                }
                em.entities[0].setStatus('w');
            }
            if (GetAsyncKeyState('S') & 0x8000) // 检查S键是否被按下，并且地图数据中相应位置为1
            {
                if ((map[(em.entities[0].getX())][em.entities[0].getY() + 1]) == '0'
                    //&& (em.entities[0].getStatus() == 's')
                )
                {
                    em.entities[0].setY(-1);
                }
                em.entities[0].setStatus('s');
            }
            if (GetAsyncKeyState('A') & 0x8000) // 检查A键是否被按下，并且地图数据中相应位置为1
            {
                if ((map[(em.entities[0].getX()) - 1][em.entities[0].getY()]) == '0'
                    //&& (em.entities[0].getStatus() == 'a')
                )
                {
                    em.entities[0].setX(-2);
                }
                em.entities[0].setStatus('a');
            }
            if (GetAsyncKeyState('D') & 0x8000) // 检查D键是否被按下，并且地图数据中相应位置为1
            {
                if ((map[(em.entities[0].getX()) + 1][(em.entities[0].getY())]) == '0'
                    //&& (em.entities[0].getStatus() == 'd')
                )
                {
                    em.entities[0].setX(-1);
                }
                em.entities[0].setStatus('d');
            }
            if (GetAsyncKeyState('J') & 0x8000) // 检查J键是否被按下
            {
                switch (em.entities[0].getStatus())
                {
                case 'a':
                    if (map[(em.entities[0].getX()) - 1][em.entities[0].getY()] != '2')
                        map[(em.entities[0].getX()) - 1][em.entities[0].getY()] = '0';
                    break;
                case 'd':
                    if (map[(em.entities[0].getX()) + 1][em.entities[0].getY()] != '2')
                        map[(em.entities[0].getX()) + 1][em.entities[0].getY()] = '0';
                    break;
                case 'w':
                    if (map[(em.entities[0].getX())][em.entities[0].getY() - 1] != '2')
                        map[(em.entities[0].getX())][em.entities[0].getY() - 1] = '0';
                    break;
                case 's':
                    if (map[(em.entities[0].getX())][em.entities[0].getY() + 1] != '2')
                        map[(em.entities[0].getX())][em.entities[0].getY() + 1] = '0';
                    break;
                default:
                    break;
                }
            }
            if (GetAsyncKeyState(VK_UP) & 0x8000)
            {
                if (map[(em.entities[1].getX())][em.entities[1].getY() - 1] == '0')
                {
                    em.entities[1].setY(-2);
                }
            }
            if (GetAsyncKeyState(VK_DOWN) & 0x8000)
            {
                if ((map[(em.entities[1].getX())][em.entities[1].getY() + 1]) == '0')
                {
                    em.entities[1].setY(-1);
                }
            }
            if (GetAsyncKeyState(VK_LEFT) & 0x8000)
            {
                if ((map[(em.entities[1].getX()) - 1][em.entities[1].getY()]) == '0')
                {
                    em.entities[1].setX(-2);
                }
            }
            if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
            {
                if ((map[(em.entities[1].getX()) + 1][(em.entities[1].getY())]) == '0')
                {
                    em.entities[1].setX(-1);
                }
            }
            if (GetAsyncKeyState(VK_SPACE) & 0x8000)
            {
                if (map[(em.entities[0].getX())][(em.entities[0].getY())] == '0')
                {
                    {
                        int virIndex = 0;
                        int virUsedNumFixed = vom.virUsedNum;
                        for (auto &virobj : vom.objects)
                        {
                            if (virobj.exist == 1 && virobj.x == em.entities[0].getX() && virobj.y == em.entities[0].getY())
                            {
                                goto stopbomb;
                            }
                            if (virIndex == virUsedNumFixed)
                            {
                                break;
                            }
                            virIndex++;
                        }
                    }
                    vom.addObject(em.entities[0].getX(), em.entities[0].getY(), 0, 0, 0, 100, 0, 100, 0);
                }
            }
            if (GetAsyncKeyState(VK_RSHIFT) & 0x8000)
            {
                if (map[(em.entities[1].getX())][(em.entities[1].getY())] == '0')
                {
                    {
                        int virIndex = 0;
                        int virUsedNumFixed = vom.virUsedNum;
                        for (auto &virobj : vom.objects)
                        {
                            if (virobj.exist == 1 && virobj.x == em.entities[1].getX() && virobj.y == em.entities[1].getY())
                            {
                                goto stopbomb;
                            }
                            if (virIndex == virUsedNumFixed)
                            {
                                break;
                            }
                            virIndex++;
                        }
                    }
                    vom.addObject(em.entities[1].getX(), em.entities[1].getY(), 0, 0, 0, 100, 0, 100, 0);
                }
            }
        stopbomb:
            if ((GetAsyncKeyState('1') & 0x8000) && (GetAsyncKeyState(VK_CONTROL) & 0x8000))
            {
                switch (em.entities[0].getStatus())
                {
                case 'a':
                    if (map[(em.entities[0].getX()) - 1][em.entities[0].getY()] != '2')
                        map[(em.entities[0].getX()) - 1][em.entities[0].getY()] = '1';
                    break;
                case 'd':
                    if (map[(em.entities[0].getX()) + 1][em.entities[0].getY()] != '2')
                        map[(em.entities[0].getX()) + 1][em.entities[0].getY()] = '1';
                    break;
                case 'w':
                    if (map[(em.entities[0].getX())][em.entities[0].getY() - 1] != '2')
                        map[(em.entities[0].getX())][em.entities[0].getY() - 1] = '1';
                    break;
                case 's':
                    if (map[(em.entities[0].getX())][em.entities[0].getY() + 1] != '2')
                        map[(em.entities[0].getX())][em.entities[0].getY() + 1] = '1';
                    break;
                default:
                    break;
                }
            }
            else if (GetAsyncKeyState('1') & 0x8000)
            {
                int tempdata;
                switch (em.entities[0].getStatus())
                {
                case 'a':
                    if ((tempdata = DetectMap(map, em.entities[0].getX(), em.entities[0].getY(), 'a', '0')) != -1)
                        map[tempdata + 1][em.entities[0].getY()] = '1';
                    break;
                case 'd':
                    if ((tempdata = DetectMap(map, em.entities[0].getX(), em.entities[0].getY(), 'd', '0')) != -1)
                        map[tempdata - 1][em.entities[0].getY()] = '1';
                    break;
                case 'w':
                    if ((tempdata = DetectMap(map, em.entities[0].getX(), em.entities[0].getY(), 'w', '0')) != -1)
                        map[em.entities[0].getX()][tempdata + 1] = '1';
                    break;
                case 's':
                    if ((tempdata = DetectMap(map, em.entities[0].getX(), em.entities[0].getY(), 's', '0')) != -1)
                        map[em.entities[0].getX()][tempdata - 1] = '1';
                    break;
                default:
                    break;
                }
            }
            if ((GetAsyncKeyState('2') & 0x8000) && (GetAsyncKeyState(VK_CONTROL) & 0x8000))
            {
                switch (em.entities[0].getStatus())
                {
                case 'a':
                    map[(em.entities[0].getX()) - 1][em.entities[0].getY()] = '2';
                    break;
                case 'd':
                    map[(em.entities[0].getX()) + 1][em.entities[0].getY()] = '2';
                    break;
                case 'w':
                    map[(em.entities[0].getX())][em.entities[0].getY() - 1] = '2';
                    break;
                case 's':
                    map[(em.entities[0].getX())][em.entities[0].getY() + 1] = '2';
                    break;
                default:
                    break;
                }
            }
            else if (GetAsyncKeyState('2') & 0x8000)
            {
                int tempdata;
                switch (em.entities[0].getStatus())
                {
                case 'a':
                    if ((tempdata = DetectMap(map, em.entities[0].getX(), em.entities[0].getY(), 'a', '0')) != -1)
                        map[tempdata + 1][em.entities[0].getY()] = '2';
                    break;
                case 'd':
                    if ((tempdata = DetectMap(map, em.entities[0].getX(), em.entities[0].getY(), 'd', '0')) != -1)
                        map[tempdata - 1][em.entities[0].getY()] = '2';
                    break;
                case 'w':
                    if ((tempdata = DetectMap(map, em.entities[0].getX(), em.entities[0].getY(), 'w', '0')) != -1)
                        map[em.entities[0].getX()][tempdata + 1] = '2';
                    break;
                case 's':
                    if ((tempdata = DetectMap(map, em.entities[0].getX(), em.entities[0].getY(), 's', '0')) != -1)
                        map[em.entities[0].getX()][tempdata - 1] = '2';
                    break;
                default:
                    break;
                }
            }
            if ((GetAsyncKeyState('3') & 0x8000) && (GetAsyncKeyState(VK_CONTROL) & 0x8000))
            {
                switch (em.entities[0].getStatus())
                {
                case 'a':
                    if (map[(em.entities[0].getX()) - 1][em.entities[0].getY()] != '2')
                        map[(em.entities[0].getX()) - 1][em.entities[0].getY()] = '3';
                    break;
                case 'd':
                    if (map[(em.entities[0].getX()) + 1][em.entities[0].getY()] != '2')
                        map[(em.entities[0].getX()) + 1][em.entities[0].getY()] = '3';
                    break;
                case 'w':
                    if (map[(em.entities[0].getX())][em.entities[0].getY() - 1] != '2')
                        map[(em.entities[0].getX())][em.entities[0].getY() - 1] = '3';
                    break;
                case 's':
                    if (map[(em.entities[0].getX())][em.entities[0].getY() + 1] != '2')
                        map[(em.entities[0].getX())][em.entities[0].getY() + 1] = '3';
                    break;
                default:
                    break;
                }
            }
            else if (GetAsyncKeyState('3') & 0x8000)
            {
                int tempdata;
                switch (em.entities[0].getStatus())
                {
                case 'a':
                    if ((tempdata = DetectMap(map, em.entities[0].getX(), em.entities[0].getY(), 'a', '0')) != -1)
                        map[tempdata + 1][em.entities[0].getY()] = '3';
                    break;
                case 'd':
                    if ((tempdata = DetectMap(map, em.entities[0].getX(), em.entities[0].getY(), 'd', '0')) != -1)
                        map[tempdata - 1][em.entities[0].getY()] = '3';
                    break;
                case 'w':
                    if ((tempdata = DetectMap(map, em.entities[0].getX(), em.entities[0].getY(), 'w', '0')) != -1)
                        map[em.entities[0].getX()][tempdata + 1] = '3';
                    break;
                case 's':
                    if ((tempdata = DetectMap(map, em.entities[0].getX(), em.entities[0].getY(), 's', '0')) != -1)
                        map[em.entities[0].getX()][tempdata - 1] = '3';
                    break;
                default:
                    break;
                }
            }
            if (GetAsyncKeyState('H') & 0x8000)
            {
                int tempdata;
                switch (em.entities[0].getStatus())
                {
                case 'a':
                    if ((tempdata = DetectMap(map, em.entities[0].getX(), em.entities[0].getY(), 'a', '0')) != -1)
                        em.entities[0].setX(tempdata + 1);
                    break;
                case 'd':
                    if ((tempdata = DetectMap(map, em.entities[0].getX(), em.entities[0].getY(), 'd', '0')) != -1)
                        em.entities[0].setX(tempdata - 1);
                    break;
                case 'w':
                    if ((tempdata = DetectMap(map, em.entities[0].getX(), em.entities[0].getY(), 'w', '0')) != -1)
                        em.entities[0].setY(tempdata + 1);
                    break;
                case 's':
                    if ((tempdata = DetectMap(map, em.entities[0].getX(), em.entities[0].getY(), 's', '0')) != -1)
                        em.entities[0].setY(tempdata - 1);
                    break;
                default:
                    break;
                }
            }
            if (GetAsyncKeyState('Q') & 0x8000) // 检查Q键是否被按下
            {
                break;
            }
            if (GetAsyncKeyState(VK_TAB) & 0x8000)
            {
                system("pause");
                break;
            }
        }

        { // 对虚拟体表vom进行遍历
            int virIndex = 0;
            int virUsedNumFixed = vom.virUsedNum;
#pragma omp parallel for num_threads(12)
            for (auto &virobj : vom.objects)
            {
                if (virobj.exist == 1)
                {
                    switch (virobj.attribute)
                    {
                    case 0:
                        if (virobj.len != 0)
                        {
                            virobj.len--;
                        }
                        else if (virobj.len == 0)
                        {
                            int tempdataa = DetectMap(map, virobj.x, virobj.y, 'a', '0');
                            int tempdatad = DetectMap(map, virobj.x, virobj.y, 'd', '0');
                            int tempdataw = DetectMap(map, virobj.x, virobj.y, 'w', '0');
                            int tempdatas = DetectMap(map, virobj.x, virobj.y, 's', '0');
                            if (abs(tempdataa - virobj.x) > 5)
                                tempdataa = virobj.x - 5;
                            if (abs(tempdatad - virobj.x) > 5)
                                tempdatad = virobj.x + 5;
                            if (abs(tempdataw - virobj.y) > 5)
                                tempdataw = virobj.y - 5;
                            if (abs(tempdatas - virobj.y) > 5)
                                tempdatas = virobj.y + 5;
                            if (map[tempdataa][virobj.y] == '3')
                            {
                                map[tempdataa][virobj.y] = '0';
                                tempdataa--;
                            }
                            if (map[tempdatad][virobj.y] == '3')
                            {
                                map[tempdatad][virobj.y] = '0';
                                tempdatad++;
                            }
                            if (map[virobj.x][tempdataw] == '3')
                            {
                                map[virobj.x][tempdataw] = '0';
                                tempdataw--;
                            }
                            if (map[virobj.x][tempdatas] == '3')
                            {
                                map[virobj.x][tempdatas] = '0';
                                tempdatas++;
                            }
                            for (int i = 0; i < abs(tempdataa - virobj.x); i++)
                            {
                                vom.addObject(virobj.x - i, virobj.y, 0, 0, 0, 100, 0, 20, 1);
                                virUsedNumFixed++;
                            }
                            for (int i = 0; i < abs(tempdatad - virobj.x); i++)
                            {
                                vom.addObject(virobj.x + i, virobj.y, 0, 0, 0, 100, 0, 20, 1);
                                virUsedNumFixed++;
                            }
                            for (int i = 0; i < abs(tempdataw - virobj.y); i++)
                            {
                                vom.addObject(virobj.x, virobj.y - i, 0, 0, 0, 100, 0, 20, 1);
                                virUsedNumFixed++;
                            }
                            for (int i = 0; i < abs(tempdatas - virobj.y); i++)
                            {
                                vom.addObject(virobj.x, virobj.y + i, 0, 0, 0, 100, 0, 20, 1);
                                virUsedNumFixed++;
                            }
                            virobj.exist = 0;
                            vom.virUsedNum--;
                        }
                        break;
                    case 1:
                        if (virobj.len != 0)
                        {
                            for (int index = 0; index < em.entities.size(); index++)
                            {
                                if (em.entities[index].getX() == virobj.x && em.entities[index].getY() == virobj.y)
                                {
                                    em.entities[index].setHp(em.entities[index].getHp() - virobj.att);
                                    if (em.entities[index].getHp() <= 0)
                                    {
                                        em.entities[index].setHp(0);
                                    }
                                }
                            }
                            virobj.len--;
                        }
                        else if (virobj.len == 0)
                        {
                            virobj.exist = 0;
                            vom.virUsedNum--;
                        }
                        break;
                    default:
                        break;
                    }
                    virIndex++;
                }
                if (virIndex == virUsedNumFixed)
                {
                    break;
                }
            }
        }

        { // 胜负推断
            if (em.entities[0].getHp() <= 0)
            {
                CONSOLE_SCREEN_BUFFER_INFO CSBI;
                GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CSBI);
                WindowDisplay resultwindow;
                resultwindow.init_New_Window((CSBI.dwSize.X - 40) / 2, (CSBI.dwSize.Y - 5) / 2, 40, 5, 0x00, 0xc);
                resultwindow.cui_Basic_Fill(' ');
                resultwindow.put_In_Text(0, 1, "           Player 1 Win!");
                resultwindow.display_Window_Str();
                Sleep(2000);
                em.entities[0].setHp(100);
                em.entities[1].setHp(100);
                break;
            }
            if (em.entities[1].getHp() <= 0)
            {
                CONSOLE_SCREEN_BUFFER_INFO CSBI;
                GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CSBI);
                WindowDisplay resultwindow;
                resultwindow.init_New_Window((CSBI.dwSize.X - 40) / 2, (CSBI.dwSize.Y - 5) / 2, 40, 5, 0x00, 0xc);
                resultwindow.cui_Basic_Fill(' ');
                resultwindow.put_In_Text(0, 1, "           Player 0 Win!");
                resultwindow.display_Window_Str();
                Sleep(2000);
                em.entities[0].setHp(100);
                em.entities[1].setHp(100);
                break;
            }
        }
        SetPosition(0, 1);
        PrintMapByRow(&map);
        // PrintMapByRange(map, em.entities[0].getX(), em.entities[0].getY()); // PrintMapByRow(&map, colindex);
        FPS.getFpsLimited();
        if (SETITEM_fpsInfoFile)
            fpsdata << frameCount << "|" << FPS.getFpsValue() << "|" << FPS.uselesscir << std::endl;
    }

    // int num = map.size();
    // std::cout << num << std::endl;

    // 文件存储操作
    WriteMapToFile(map, "areamap.txt");
    em.writeEntitiesToFile("entities.txt");
    if (vom.virUsedNum != 0)
        vom.writeVirtualObjectsToFile("virtualobjects.txt");
    fpsdata.close();
    loginfo.close();
    return 0;
}
void ReadSetInfoFromFile(void)
{
    std::ifstream file("settings.txt");
    if (file.is_open())
    {
        std::string line;
        while (std::getline(file, line))
        {
            std::vector<std::string> fields = split(line, ':');
            if (fields.size() == 2)
            {
                setCoreWindowItem.push_back(fields[0]);
                setCoreWindowSelect.push_back(fields[1]);
            }
            else
            {
                // 无效的设置行
                loginfo << FormatTime(time(nullptr)) << "Invalid settings line: " << line << std::endl;
            }
        }
        file.close();
    }
    EffectSetInfo();
}
void SaveSetInfoToFile(void)
{
    std::ofstream file("settings.txt");
    if (file.is_open())
    {
        for (size_t i = 0; i < setCoreWindowItem.size(); i++)
        {
            file << setCoreWindowItem[i] << ":" << setCoreWindowSelect[i] << std::endl;
        }
        file.close();
    }
}
void SetModule(void)
{
    system("cls");
    CONSOLE_SCREEN_BUFFER_INFO CSBI;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CSBI);
    WindowDisplay setCoreWindow;

    const int setWindowWidth = 60;
    const int setWindowHeight = 30;
    int highlightItem = 0;
    bool changeState = true;
    bool bottom_pressed_permisson = true;
    setCoreWindow.init_New_Window((CSBI.dwSize.X - setWindowWidth) / 2, (CSBI.dwSize.Y - setWindowHeight) / 2, setWindowWidth, setWindowHeight, 0x7, 0x0);
    setCoreWindow.cui_Basic_Fill(' ');

    std::cout << "\e[" << ((CSBI.dwSize.Y - setWindowHeight) / 2 + 2) << ";" << ((CSBI.dwSize.X - 8) / 2 + 2) << "H";
    std::cout << "设置页";
    std::cout << "\e[" << ((CSBI.dwSize.Y - setWindowHeight) / 2 + 3) << ";" << ((CSBI.dwSize.X - setWindowWidth) / 2 + 1) << "H";
    std::cout << "┣";
    for (int index = 0; index < setWindowWidth - 2; index++)
    {
        std::cout << "\e[37m"
                  << "━"
                  << "\e[0m";
    }
    std::cout << "┫";

    while (true)
    {
        Sleep(1);
        if (changeState)
            for (int index = 0; index < setCoreWindowItem.size(); index++)
            {
                if (index == highlightItem)
                {
                    std::cout << "\e[" << ((CSBI.dwSize.Y - setWindowHeight) / 2 + 4 + index)
                              << ";" << ((CSBI.dwSize.X - setWindowWidth) / 2 + 2) << "H";
                    for (int indexx = 0; indexx < setWindowWidth - 2; indexx++)
                    {
                        std::cout << "\e[46m"
                                  << " "
                                  << "\e[0m";
                    }
                }
                else
                {
                    std::cout << "\e[" << ((CSBI.dwSize.Y - setWindowHeight) / 2 + 4 + index)
                              << ";" << ((CSBI.dwSize.X - setWindowWidth) / 2 + 2) << "H";
                    for (int indexx = 0; indexx < setWindowWidth - 2; indexx++)
                    {
                        std::cout << "\e[0m"
                                  << " ";
                    }
                }
                if (index == highlightItem)
                {
                    std::cout << "\e[" << ((CSBI.dwSize.Y - setWindowHeight) / 2 + 4 + index)
                              << ";" << ((CSBI.dwSize.X - setWindowWidth) / 2 + 2) << "H";
                    std::cout << "\e[30m\e[46m" << setCoreWindowItem[index] << "\e[0m";
                }
                else
                {
                    std::cout << "\e[" << ((CSBI.dwSize.Y - setWindowHeight) / 2 + 4 + index)
                              << ";" << ((CSBI.dwSize.X - setWindowWidth) / 2 + 2) << "H";
                    std::cout << "\e[37m" << setCoreWindowItem[index] << "\e[0m";
                }

                if (index == highlightItem)
                {
                    std::cout << "\e[" << ((CSBI.dwSize.Y - setWindowHeight) / 2 + 4 + index)
                              << ";" << ((CSBI.dwSize.X - setWindowWidth) / 2 + 2 + setWindowWidth - 8) << "H";
                    std::cout << "\e[30m\e[46m" << setCoreWindowSelect[index] << "\e[0m";
                }
                else
                {
                    std::cout << "\e[" << ((CSBI.dwSize.Y - setWindowHeight) / 2 + 4 + index)
                              << ";" << ((CSBI.dwSize.X - setWindowWidth) / 2 + 2 + setWindowWidth - 8) << "H";
                    std::cout << "\e[37m" << setCoreWindowSelect[index] << "\e[0m";
                }
                std::cout.flush();
                changeState = false;
            }

        if ((GetAsyncKeyState(VK_UP) & 0x8000) && bottom_pressed_permisson)
        {
            bottom_pressed_permisson = false;
            changeState = true;
            highlightItem--;
            if (highlightItem < 0)
            {
                highlightItem = setCoreWindowItem.size() - 1;
            }
        }
        if ((GetAsyncKeyState(VK_DOWN) & 0x8000) && bottom_pressed_permisson)
        {
            bottom_pressed_permisson = false;
            changeState = true;
            highlightItem++;
            if (highlightItem > setCoreWindowItem.size() - 1)
            {
                highlightItem = 0;
            }
        }
        if ((GetAsyncKeyState(VK_RETURN) & 0x8000) && bottom_pressed_permisson)
        {
            bottom_pressed_permisson = false;
            changeState = true;
            if (setCoreWindowSelect[highlightItem] == "Y")
            {
                setCoreWindowSelect[highlightItem] = "N";
            }
            else if (setCoreWindowSelect[highlightItem] == "N")
            {
                setCoreWindowSelect[highlightItem] = "Y";
            }
        }
        if ((GetAsyncKeyState(VK_LEFT) & 0x8000) && bottom_pressed_permisson && (GetAsyncKeyState(VK_SHIFT) & 0x8000))
        {
            bottom_pressed_permisson = false;
            changeState = true;
            if (IsNumber(setCoreWindowSelect[highlightItem]))
            {
                int temp = std::stoi(setCoreWindowSelect[highlightItem]);
                temp = temp - 10;
                if (temp < 0)
                {
                    temp = 0;
                }
                setCoreWindowSelect[highlightItem] = std::to_string(temp);
            }
        }
        else if ((GetAsyncKeyState(VK_LEFT) & 0x8000) && bottom_pressed_permisson)
        {
            bottom_pressed_permisson = false;
            changeState = true;
            if (IsNumber(setCoreWindowSelect[highlightItem]))
            {
                int temp = std::stoi(setCoreWindowSelect[highlightItem]);
                temp--;
                if (temp < 0)
                {
                    temp = 0;
                }
                setCoreWindowSelect[highlightItem] = std::to_string(temp);
            }
        }
        if ((GetAsyncKeyState(VK_RIGHT) & 0x8000) && bottom_pressed_permisson && (GetAsyncKeyState(VK_SHIFT) & 0x8000))
        {
            bottom_pressed_permisson = false;
            changeState = true;
            if (IsNumber(setCoreWindowSelect[highlightItem]))
            {
                int temp = std::stoi(setCoreWindowSelect[highlightItem]);
                temp = temp + 10;
                if (temp > 1000)
                {
                    temp = 1000;
                }
                setCoreWindowSelect[highlightItem] = std::to_string(temp);
            }
        }
        else if ((GetAsyncKeyState(VK_RIGHT) & 0x8000) && bottom_pressed_permisson)
        {
            bottom_pressed_permisson = false;
            changeState = true;
            if (IsNumber(setCoreWindowSelect[highlightItem]))
            {
                int temp = std::stoi(setCoreWindowSelect[highlightItem]);
                temp++;
                if (temp > 1000)
                {
                    temp = 1000;
                }
                setCoreWindowSelect[highlightItem] = std::to_string(temp);
            }
        }

        if ((GetAsyncKeyState(VK_ESCAPE) & 0x8000))
        {
            break;
        }
        if (!((GetAsyncKeyState(VK_SPACE) & 0x8000) ||
              (GetAsyncKeyState(VK_RETURN) & 0x8000) ||
              (GetAsyncKeyState(VK_UP) & 0x8000) ||
              (GetAsyncKeyState(VK_DOWN) & 0x8000) ||
              (GetAsyncKeyState(VK_LEFT) & 0x8000) ||
              (GetAsyncKeyState(VK_RIGHT) & 0x8000)))
        {
            bottom_pressed_permisson = true;
        }
    }

    EffectSetInfo();
}
void EffectSetInfo()
{

    if (setCoreWindowSelect[0] == "Y")
    {
        SETITEM_windowPreDisplay = true;
    }
    else if (setCoreWindowSelect[0] == "N")
    {
        SETITEM_windowPreDisplay = false;
    }
    if (setCoreWindowSelect[1] == "Y")
    {
        SETITEM_fpsInfoFile = true;
    }
    else if (setCoreWindowSelect[1] == "N")
    {
        SETITEM_fpsInfoFile = false;
    }
    SETITEM_limitedFps = std::stoi(setCoreWindowSelect[2]);
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

    // int cols = WIN_MENU_LONAXIS;      // 指定列数
    // int lines = WIN_MENU_HORAXIS - 1; // 指定行数
    // std::string command = "mode con cols=" + std::to_string(cols) + " lines=" + std::to_string(lines);
    // system(command.c_str());

    // SMALL_RECT windowSize = {0, 0, WIN_MENU_LONAXIS - 1, WIN_MENU_HORAXIS - 2};
    // SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, &windowSize);
    // COORD bufferSize = {WIN_MENU_LONAXIS, WIN_MENU_HORAXIS - 1};
    // SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), bufferSize);

    // 窗口置顶
    if (setCoreWindowSelect[0] == "Y")
    {
        SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_DRAWFRAME);
    }
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
}
int main(void)
{
    ReadSetInfoFromFile();
    bool is_esc_permitted = true;
    InitMainDrive();
printMenu_:
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    int windowLeftPointX;
    windowLeftPointX = (csbi.dwSize.X - WIN_MENU_LONAXIS) / 2;
    int windowLeftPointY;
    windowLeftPointY = (csbi.dwSize.Y - WIN_MENU_HORAXIS) / 2;
    loginfo << "windowLeftPointX: " << windowLeftPointX << std::endl;
    for (int index = 1; index < 13; index++)
    {
        std::cout << "\033[" << windowLeftPointY + index << ";" << windowLeftPointX + 1 << "H";
        for (int indexx = 1; indexx < WIN_MENU_LONAXIS; indexx++)
        {
            std::cout << "\e[106m"
                      << " "
                      << "\e[0m";
        }
        std::cout.flush();
    }
    for (int index = 14; index < WIN_MENU_HORAXIS - 1; index++)
    {
        std::cout << "\033[" << windowLeftPointY + index << ";" << windowLeftPointX + 1 << "H";
        for (int indexx = 1; indexx < WIN_MENU_LONAXIS; indexx++)
        {
            std::cout << " ";
        }
        std::cout.flush();
    }
    std::cout << "\033[0m";

    std::cout << "\033[" << windowLeftPointY + 1 << ";" << windowLeftPointX << "H";
    std::string logoStyle[12] = {

        "           ___           ___                       ___     ",
        "          /\\  \\         /\\  \\          ___        /\\  \\ ",
        "         /  \\  \\       /  \\  \\        /\\  \\      /  \\  \\ ",
        "        / /\\ \\  \\     / /\\ \\  \\       \\ \\  \\    / /\\ \\  \\ ",
        "       /  \\ \\ \\__\\   _\\ \\ \\ \\  \\      /  \\__\\  /  \\ \\ \\  \\ ",
        "      / /\\ \\ \\ |__| /\\ \\ \\ \\ \\__\\  __/ /\\/__/ / /\\ \\ \\ \\__\\ ",
        "      \\ \\ \\ \\/ /  / \\ \\ \\ \\ \\/__/ /\\/ /  /    \\/__\\ \\ \\/__/",
        "       \\ \\ \\  /  /   \\ \\ \\ \\__\\   \\  /__/          \\ \\__\\ ",
        "        \\ \\/ /  /     \\ \\/ /  /    \\ \\__\\           \\/__/",
        "         \\  /__/       \\  /  /      \\/__/                  ",
        "          ~~            \\/__/               ",
        "                                                           ",
    };
    for (int index = 0; index < 12; index++)
        std::cout << "\033[" << windowLeftPointX << "G"
                  << "\033[38;5;205m\033[1m\e[106m" << logoStyle[index] << "\033[0m"
                  << "\033[1E";
    std::cout.flush();

    std::cout << "\033[" << windowLeftPointY + 13 << ";" << windowLeftPointX + 1 << "H";
    std::cout << "\033[33m\033[1m\033[46m"
              << "-------------------" << VERSION_TXT << "---------------------"
              << "\033[0m"
              << "\033[36m\033[1m"
              << "\e[2E\e[" << windowLeftPointX + (WIN_MENU_LONAXIS - 8) / 2 << "G"
              << "1.测试"
              << "\e[2E\e[" << windowLeftPointX + (WIN_MENU_LONAXIS - 8) / 2 << "G"
              << "2.关于"
              << "\e[2E\e[" << windowLeftPointX + (WIN_MENU_LONAXIS - 8) / 2 << "G"
              << "S.设置"
              << "\e[2E\e[" << windowLeftPointX + (WIN_MENU_LONAXIS - 8) / 2 << "G"
              << "Q.退出"
              << "\033[0m";
    std::cout.flush();

    std::cout << "\033[" << windowLeftPointY + 0 << ";" << windowLeftPointX << "H";
    for (int i = 0; i <= WIN_MENU_LONAXIS - 1; i++)
        std::cout << "\033[32m" << '=' << "\033[0m";
    std::cout.flush();

    std::cout << "\033[" << windowLeftPointY + WIN_MENU_HORAXIS - 2 << ";" << windowLeftPointX << "H";
    for (int i = 0; i <= WIN_MENU_LONAXIS - 1; i++)
        std::cout << "\033[32m" << '=' << "\033[0m";
    std::cout.flush();

    for (int i = 0; i <= WIN_MENU_HORAXIS - 2; i++)
    {
        std::cout << "\033[" << windowLeftPointY + i << ";" << windowLeftPointX << "H";
        std::cout << "|";
        std::cout.flush();
    }
    for (int i = 0; i <= WIN_MENU_HORAXIS - 2; i++)
    {
        std::cout << "\033[" << windowLeftPointY + i << ";" << windowLeftPointX + WIN_MENU_LONAXIS << "H";
        std::cout << "|";
        std::cout.flush();
    }
    int tempWindowLeftPointX = csbi.dwSize.X;
    int tempWindowLeftPointY = csbi.dwSize.Y;
    while (true)
    {
        Sleep(20);
        if (time(0) % 1 == 0)
        {
            GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
            if ((csbi.dwSize.X > WIN_MENU_LONAXIS + 1 && tempWindowLeftPointX <= WIN_MENU_LONAXIS + 1) || (csbi.dwSize.Y > WIN_MENU_HORAXIS && tempWindowLeftPointY <= WIN_MENU_HORAXIS))
            {
                system("cls");
                goto printMenu_;
            }
            if (csbi.dwSize.X <= WIN_MENU_LONAXIS + 1 || csbi.dwSize.Y < WIN_MENU_HORAXIS)
            {
                tempWindowLeftPointX = csbi.dwSize.X;
                tempWindowLeftPointY = csbi.dwSize.Y;
                if (csbi.dwSize.Y <= 5)
                {
                    system("cls");
                    continue;
                }
                system("cls");
                WindowDisplay warningWindow;
                warningWindow.init_New_Window((csbi.dwSize.X - 26) / 2, (csbi.dwSize.Y - 5) / 2, 26, 5, 0x0, 0xc);
                warningWindow.cui_Basic_Fill(' ');
                warningWindow.put_In_Text(0, 1, " Window Size is Invalid");
                warningWindow.display_Window_Str();
                continue;
            }

            if (csbi.dwSize.X != tempWindowLeftPointX || csbi.dwSize.Y != tempWindowLeftPointY)
            {
                tempWindowLeftPointX = csbi.dwSize.X;
                tempWindowLeftPointY = csbi.dwSize.Y;
                system("cls");
                goto printMenu_;
            }
        }
        if (GetAsyncKeyState('1') & 0x8000) // 检查1键是否被按下
        {
            CoreCircle();
            system("cls");
            goto printMenu_;
        }
        if (GetAsyncKeyState('2') & 0x8000) // 检查2键是否被按下
        {
            MessageBoxExA(GetConsoleWindow(), "Copyright 2023 Zhou Xu.All rights reserved.", (VERSION_TXT), MB_OK | MB_ICONASTERISK | MB_TOPMOST, 0);
            continue;
        }
        if (GetAsyncKeyState('T') & 0x8000) // 检查T键是否被按下
        {
            WindowDisplay setWindow;

            setWindow.init_New_Window(10, 10, 35, 8, 0x0, 0xb);
            setWindow.cui_Basic_Fill(' ');
            setWindow.put_In_Text(0, 0, "测试");
            setWindow.display_Window_Str();
            continue;
        }
        if (GetAsyncKeyState('S') & 0x8000) // 检查S键是否被按下
        {
            SetModule();
        }
        if (GetAsyncKeyState('Q') & 0x8000) // 检查Q键是否被按下
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
    SaveSetInfoToFile();
    return 0;
}