#define VERSION_TXT " 6.1.0 - PRE - 121223P1 "
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
#define WIN_MENU_LONAXIS 64 // 横长
#define WIN_MENU_HORAXIS 25 // 纵长
HANDLE hOutStd;
HWND hwnd;
bool is_esc_permitted = true;

void readMapFromFile(std::vector<std::array<char, 64>> &map, const std::string &filename);
void writeMapToFile(const std::vector<std::array<char, 64>> &map, const std::string &filename);
void SetColor(UINT uFore, UINT uBack);
void PrintMapByCol(const std::vector<std::array<char, 64>> *map);
void PrintMapByRow(const std::vector<std::array<char, 64>> *map);
void PrintMapByRange(const std::vector<std::array<char, 64>> &map, int rowIdx, int colIdx);
void AppendMapToFile(const std::string &filename);
bool SetPosition(int x, int y);
void SetConsoleWindowPosition(int x, int y);
bool OpenANSIControlChar();
void InitTestEnv();
int CoreCircle(void);
void InitMainDrive(void);
void InitMainDrive(void);

// (6.1.0 - preRelease - 120921P1):更新了图形显示函数,在基本不损失性能的前提下实现彩色字符输出
// (6.1.0 - preRelease - 121014P1):更新了新的菜单主界面和logo
// (6.1.0 - preRelease - 121022P1):开始编写终端窗口类
// (6.1.0 - preRelease - 121120P1):尝试使用PDCurses库重写图形程序,新的源代码已经移至PDCurses分支
// (6.1.0 - preRelease - 121121P1):新增了FPS日志输出,用以检测程序性能
// (6.1.0 - preRelease - 121201P1):重写了windowDisplay类，实现在子窗口中输出文本的功能
// (6.1.0 - preRelease - 121220P1):继续完善windowDisplay类，实现了子窗口的背景色
// (6.1.0 - preRelease - 121220P2):已定位BFX-2023-1212C1-N:终端内运行时菜单窗口的显示出现问题,原因时终端对窗口缓冲区的API响应存在问题
// (6.1.0 - preRelease - 121223P1):新增了实体类和虚拟体类