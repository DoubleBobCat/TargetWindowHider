//
// Created by DoubleCat.
//
#include <windows.h>

void GetTargetWindowInfo(int FindWindowMode, HWND__ *TargetWindowAddress, LPCSTR TargetClassName, LPCSTR TargetWindowName,RECT ProgramWindowInfoData, RECT TargetWindowInfoData, RECT ScreenData) {
    if (FindWindowMode == 0)
        TargetWindowAddress = FindWindow(nullptr, TargetWindowName);
    else if (FindWindowMode == 1)
        TargetWindowAddress = FindWindow(TargetClassName, nullptr);
    else
        TargetWindowAddress = FindWindow(TargetClassName, TargetWindowName);
    if (::GetWindowRect(GetConsoleWindow(), &ProgramWindowInfoData))
        if (ProgramWindowInfoData.left == -25600 || ProgramWindowInfoData.top == -25600) {
            ::SetWindowPos(GetConsoleWindow(), nullptr, 160, 90, 480, 270, SWP_HIDEWINDOW);
            SetProgramWindowShow(0);
        }
    if (TargetWindowAddress != nullptr) {
        if (::GetWindowRect(TargetWindowAddress, &TargetWindowInfoData) != 0) {
            if (TargetWindowInfoData.left == -26500 || TargetWindowInfoData.top == -25600) {
                std::cout << "目标程序处于最小化，请取消目标程序的最小化状态。" << std::endl;
                PrintTime++;
            } else {
                TargetWindowInfo[0].x = TargetWindowInfoData.right - TargetWindowInfoData.left;
                TargetWindowInfo[0].y = TargetWindowInfoData.bottom - TargetWindowInfoData.top;
                if (TargetWindowInfoData.right <= 0) {
                    TargetWindowInfo[0].flag = 0;
                    TargetWindowShowFlag = false;
                } else if (TargetWindowInfoData.left >= ScreenInfo.x) {
                    TargetWindowInfo[0].flag = 1;
                    TargetWindowShowFlag = false;
                } else if ((TargetWindowInfoData.left > 5 || TargetWindowInfo[0].x < (ScreenInfo.x - 5)) && TargetWindowInfoData.bottom <= 0) {
                    TargetWindowInfo[0].flag = 2;
                    TargetWindowShowFlag = false;
                } else if (TargetWindowInfoData.left <= 0) {
                    TargetWindowInfo[0].flag = 0;
                    TargetWindowShowFlag = true;
                } else if (TargetWindowInfoData.right >= (ScreenInfo.x - 5)) {
                    TargetWindowInfo[0].flag = 1;
                    TargetWindowShowFlag = true;
                } else if (TargetWindowInfoData.top <= 5) {
                    TargetWindowInfo[0].flag = 2;
                    TargetWindowShowFlag = true;
                } else {
                    TargetWindowInfo[0].flag = -1;
                    TargetWindowShowFlag = true;
                }
            }
        } else {
            std::cout << "无法获取目标程序信息，请检查是否目标程序是否打开或类名、窗口名有误，" << "Error_code: " << GetLastError() << "。" << std::endl;
            PrintTime++;
        }
    } else {
        std::cout << "无法获取目标程序信息，请检查是否目标程序是否打开或类名、窗口名有误，" << "Error_code: " << GetLastError() << "。" << std::endl;
        PrintTime++;
    }
    GetMouseInfo();
    Sleep(10);
}


//...

HWND hWnd = /* your window handle here */;

// Get the monitor that contains the specified window
HMONITOR hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTOPRIMARY);

// Get the monitor info for the specified monitor
MONITORINFO monitorInfo;
monitorInfo.
cbSize = sizeof(MONITORINFO);
GetMonitorInfo(hMonitor, &monitorInfo
);

// The monitor's position and size are now in monitorInfo.rcMonitor.
// You can use this to determine the screen size and position.

// Example usage:
int screenWidth = monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left;
int screenHeight = monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top;
