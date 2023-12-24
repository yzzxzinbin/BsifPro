#include <iostream>
#include <windows.h>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
using namespace std;
int main(void)
{

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    cout << csbi.dwSize.X << endl;
    cout << csbi.dwSize.Y << endl;
    cout << csbi.dwCursorPosition.X << endl;
    cout << csbi.dwCursorPosition.Y << endl;
    Sleep(1000);
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    cout << csbi.dwSize.X << endl;
    cout << csbi.dwSize.Y << endl;
    cout << csbi.dwCursorPosition.X << endl;
    cout << csbi.dwCursorPosition.Y << endl;
    csbi.dwSize.X = 100;
    SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), csbi.dwSize);
    Sleep(2000);
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    cout << csbi.dwSize.X << endl;
    cout << csbi.dwSize.Y << endl;
    cout << csbi.dwCursorPosition.X << endl;
    cout << csbi.dwCursorPosition.Y << endl;
    cin.get();
    return 0;
}