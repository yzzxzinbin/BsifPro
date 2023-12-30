#define VERSION_TXT " 6.1.1 - PRE - 123102P2 "
#include <windows.h>
#include <vector>
#include <array>
#include <iostream>
#include <fstream>
#include <random>
#include <filesystem>
#include <ctime>
#include <string>

#define WIN_MENU_LONAXIS 64 // 横长
#define WIN_MENU_HORAXIS 25 // 纵长
HANDLE hOutStd;
HWND hwnd;

std::vector<std::string> setCoreWindowItem;
std::vector<std::string> setCoreWindowSelect;
std::ofstream loginfo("loginfo.txt");

bool SETITEM_windowPreDisplay = true;
bool SETITEM_fpsInfoFile = true;
int SETITEM_limitedFps = 60;

std::string FormatTime(time_t time);
bool IsNumber(const std::string &str);
void ReadMapFromFile(std::vector<std::vector<char>> &map, const std::string &filename);
void WriteMapToFile(const std::vector<std::vector<char>> &map, const std::string &filename);
void SetColor(UINT uFore, UINT uBack);
COORD GetColor(void);
void PrintMapByCol(const std::vector<std::vector<char>> *map);
void PrintMapByRow(const std::vector<std::vector<char>> *map);
void PrintMapByRange(const std::vector<std::vector<char>> &map, int rowIdx, int colIdx);
void AppendMapToFile(const std::string &filename);
bool SetPosition(int x, int y);
void SetConsoleWindowPosition(int x, int y);
bool OpenANSIControlChar();
int DetectMap(const std::vector<std::vector<char>> &map, int x, int y, char status, char symbol);
std::vector<std::string> split(const std::string &str, char delimiter);
void InitTestEnv();
int CoreCircle(void);
void ReadSetInfoFromFile(void);
void SaveSetInfoToFile(void);
void SetModule(void);
void EffectSetInfo();
void InitMainDrive(void);
void InitMainDrive(void);

//------------ BSIF 6 起步版本 --------------
// 该版本主要使用STL编写图像显示工具和工程基本数据结构类
// (6.1.0 - preRelease - 120921P1):更新移动图形显示函数,基本不损失性能的前提下实现彩色字符输出
// (6.1.0 - preRelease - 121014P1):更新新的菜单主界面和logo
// (6.1.0 - preRelease - 121022P1):开始编写终端窗口类
// (6.1.0 - preRelease - 121120P1):尝试使用PDCurses库重写图形程序,新的源代码已经移至PDCurses分支
// (6.1.0 - preRelease - 121121P1):新增了FPS日志输出,用以检测程序性能
// (6.1.0 - preRelease - 121201P1):重写了windowDisplay类，实现在子窗口中输出文本的功能
// (6.1.0 - preRelease - 121220P1):继续完善windowDisplay类，实现了子窗口的背景色
// (6.1.0 - preRelease - 121220P2):已定位BFX-2023-1212C1-N:终端内运行时菜单窗口的显示出现问题,原因是终端对窗口缓冲区的API响应存在问题
// (6.1.0 - preRelease - 121223P1):新增了实体类和虚拟体类
// (6.1.0 - preRelease - 121323P1):完成了实体类和虚拟体类的文件读写模块
// (6.1.0 - preRelease - 121400P1):新增日志输出流
//------------ BSIF 6.1 PPT工程 --------------
// (6.1.1 - preRelease - 121412P1):修正并完善了行列图形显示函数,为第一个项目泡泡堂作准备
// (6.1.1 - preRelease - 121417P1):重写地图数据结构为双重嵌套向量,并对相应API函数做出了调整
// (6.1.1 - preRelease - 121420P1):编译时添加了程序ico,调整了显示参数
// (6.1.1 - preRelease - 121421P1):编写实体操作逻辑
// (6.1.1 - preRelease - 121501P1):编写地图编辑工具
// (6.1.1 - preRelease - 121515P1):继续完善地图编辑工具基本功能,新增了新的位移方式
// (6.1.1 - preRelease - 121523P1):开始编写虚拟体相关代码,包括文件读写,显示,逻辑等
// (6.1.1 - preRelease - 121622P1):已修复一类虚拟体逻辑BUG
// (6.1.1 - preRelease - 121623P1):使用转义字符序列对图形显示进行了优化
// (6.1.1 - preRelease - 121701P1):已定位BFX-2023-1217X1-N: 对虚拟体表同时作注销和注册操作时会引发坐标错误
// (6.1.1 - preRelease - 122217P1):已修复BFX-2023-1217X1-Y: 对虚拟体表同时作注销和注册操作时会引发坐标错误
// (6.1.1 - preRelease - 122222P1):已修复BFX-2023-1222X1-Y: 行输出函数对于虚拟体表的遍历逻辑错误导致帧率异常
// (6.1.1 - preRelease - 122317P1):新增虚拟体地形破坏逻辑
// (6.1.1 - preRelease - 122317P2):初步尝试编写虚拟体和实体属性交互逻辑
// (6.1.1 - preRelease - 122323P1):新增设置模块
// (6.1.1 - preRelease - 122415P1):设置页面逻辑基本完成
// (6.1.1 - preRelease - 122817P1):使用winmm定时器精度API实现了高精度休眠时控,CPU占用率降低95%
// (6.1.1 - preRelease - 122817P2):通过填充对齐彻底修复windowDisplay的文本残留问题
// (6.1.1 - preRelease - 122820P1):增加设置文件读写函数,并将设置UI和设置逻辑对接
// (6.1.1 - preRelease - 123021P1):增加了控制台窗口固定
// (6.1.1 - preRelease - 123022P1):增加了血量交互,增加了结束画面
// (6.1.1 - preRelease - 123023P1):经过大量测试,确认BFX-2023-1212C1-N是由缓冲区规格和窗口大小不一致导致的
// (6.1.1 - preRelease - 123001P1):使用转义字符序列重写了主菜单界面
// (6.1.1 - preRelease - 123102P1):修复了BFX-2023-1212C1-Y:增加了主菜单窗口位置自适应支持,并在无法适应时作出提示
// (6.1.1 - preRelease - 123102P2):修复了BFX-2023-1230A1-Y:修复了窗口最大化切换时,窗口自适应失效的问题