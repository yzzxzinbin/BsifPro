#define VERSION_TXT " 6.1.0 - PRE - 121201P1 "
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