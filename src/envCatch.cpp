#include <windows.h>
#include <tlhelp32.h>
#include <stdbool.h>
#include <string>
#include <iostream>
HWND hwnd;
std::string ConvertWideCharToMultiByte(const WCHAR *wideCharString)
{
    int bufferSize = WideCharToMultiByte(CP_UTF8, 0, wideCharString, -1, nullptr, 0, nullptr, nullptr);
    if (bufferSize == 0)
    {
        return ""; // 转换失败
    }
    static std::string charString(bufferSize, '\0');
    WideCharToMultiByte(CP_UTF8, 0, wideCharString, -1, &charString[0], bufferSize, nullptr, nullptr);
    return charString;
}
bool ExistProcess(LPCSTR lpName) // 判断是否存在指定进程
{
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (INVALID_HANDLE_VALUE == hSnapshot)
    {
        return false;
    }
    PROCESSENTRY32 pe = {sizeof(pe)};
    BOOL fOk;
    for (fOk = Process32First(hSnapshot, &pe); fOk; fOk = Process32Next(hSnapshot, &pe))
    {

        if (!stricmp(pe.szExeFile, lpName))
        {
            CloseHandle(hSnapshot);
            return true;
        }
    }
    return false;
}

bool TerminalCheck(DWORD dwPid, HWND _hwnd)
{
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (INVALID_HANDLE_VALUE == hSnapshot)
    {
        return false;
    }
    PROCESSENTRY32 pe = {sizeof(pe)};
    BOOL fOk;
    for (fOk = Process32First(hSnapshot, &pe); fOk; fOk = Process32Next(hSnapshot, &pe))
    {
        if (!stricmp(pe.szExeFile, "WindowsTerminal.exe") && pe.th32ProcessID == dwPid)
        {
            CloseHandle(hSnapshot);
            WCHAR title[MAX_PATH];
            GetWindowTextW(_hwnd, title, MAX_PATH);
            std::string convertedTitle = ConvertWideCharToMultiByte(title);
            if (strcmp(convertedTitle.c_str(), _pgmptr) && strcmp(convertedTitle.c_str(), "envCatch.exe"))
            {
                return false;
            }
            return true;
        }
    }
    return false;
}
BOOL CALLBACK EnumWindowsProc(HWND _hwnd, LPARAM lParam)
{
    DWORD pid;
    GetWindowThreadProcessId(_hwnd, &pid);
    if (TerminalCheck(pid, _hwnd))
    {
        printf("Terminal Found! pid=%ld hwnd=%p\n", pid, _hwnd);
        hwnd = _hwnd;
        return FALSE;
    }
    return TRUE;
}

int main(int argc, char *argv[])
{
    std::string title = "envCatch.exe";
    SetConsoleTitleA(title.c_str());
    if (ExistProcess("WindowsTerminal.exe"))
    {
        EnumWindows(EnumWindowsProc, 0);
        int x = 0;        // 窗口左上角 x 坐标
        int y = 0;        // 窗口左上角 y 坐标
        int width = 100;  // 窗口宽度
        int height = 100; // 窗口高度
        // 调用 SetWindowPos 函数
        SetWindowPos(hwnd, NULL, x, y, width, height, SWP_NOZORDER | SWP_SHOWWINDOW);
    }
    else
    {
        hwnd = GetConsoleWindow();
    }
    if (!hwnd || hwnd == INVALID_HANDLE_VALUE)
    {
        hwnd = GetForegroundWindow();
        printf("Failed! using GetForegroundWindow...");
    }
    std::cin.get();
    return 0;
}