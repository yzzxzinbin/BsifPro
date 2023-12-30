#include <iostream>
#include <windows.h>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#define WIN_MENU_LONAXIS 64 // 横长
#define WIN_MENU_HORAXIS 25 // 纵长
#define VERSION_TXT " 6.1.1 - PRE - 122415P1 "
using namespace std;
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
    for (int index = 0; index < 255; index++)
    {
        cout << "\033[38;5;" << index << "m" << index << "\033[0m" << "\t";
        // if (index % 16 == 0)
        //     cout << endl;
    }
    cin.get();
    return 0;
}