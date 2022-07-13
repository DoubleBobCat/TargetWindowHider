#include <iostream>
#include <windows.h>
#include <cstring>
#include <cmath>

#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0)
#define TargetClassName "WeChatMainWndForPC"
#define TargetWindowName nullptr

struct WindowInfo {
    int x, y, a;
} TargetWindowInfo[2], ScreenInfo, MouseInfoForTargetWindow;

long long GetTargetWindowInfo();//获取微信窗口位置
void SetTargetWindowSide(int mod, int point);//移动微信窗口位置
void SetProgramWindowShow(int mod);//设置窗口显示情况
void GetMouseInfoForTargetWindow();//获取鼠标相对窗口位置

HWND TargetWindowAddress;
RECT TargetWindowInfoData, ScreenData;
POINT MousePoint, MousePointBuffer;
size_t PrintTime = 0;
bool TargetWindowFirstFindFlag = true, TargetWindowFlag = true;

int main() {
    system("chcp 65001");//设置程序语言为中文
    SetProgramWindowShow(0);
    SetWindowText(GetConsoleWindow(), "WindowHider");
    std::cout << "程序开始运行……" << std::endl;
    if (!::SystemParametersInfoA(SPI_GETWORKAREA, 0, &ScreenData, 0)) {
        SetProgramWindowShow(1);
        std::cout << "无法获取屏幕信息，请联系开发者。Error_code: " << GetLastError() << std::endl << "程序将在6s后自动关闭";
        Sleep(6000);
        exit(0);
    }
    ScreenInfo.x = ScreenData.right - ScreenData.left;
    ScreenInfo.y = ScreenData.bottom - ScreenData.top;
    MousePointBuffer.x = -25600;
    TargetWindowInfo[0].a = -1;
    while (true) {
        long long GetTargetWindowInfoCode = GetTargetWindowInfo();

        if (GetTargetWindowInfoCode != -1) {
            if (GetTargetWindowInfoCode == -25600) {
                std::cout << "目标程序处于最小化，请取消微信的最小化状态。" << std::endl;
            } else {
                std::cout << "无法获取目标程序信息，请检查是否目标程序是否打开或联系开发者，"
                          << "Error_code: " << GetTargetWindowInfoCode << "。" << std::endl;
            }
        } else {
            if (TargetWindowFirstFindFlag) {
                TargetWindowInfo[1].x = TargetWindowInfoData.left;
                TargetWindowInfo[1].y = TargetWindowInfoData.top;
                TargetWindowInfo[1].a = TargetWindowInfoData.bottom - TargetWindowInfoData.top;
                TargetWindowFirstFindFlag = false;
            }
            if (GetCursorPos(&MousePoint) == 0) {
                SetProgramWindowShow(1);
                std::cout << "无法获取鼠标信息，请联系开发者。Error_code: " << GetLastError() << std::endl << "程序将在6s后自动关闭";
                Sleep(6000);
                exit(0);
            } else {
                if (TargetWindowFlag) {
                    if (MousePoint.x <= TargetWindowInfoData.right && MousePoint.x >= TargetWindowInfoData.left &&
                        MousePoint.y <= TargetWindowInfoData.bottom && MousePoint.y >= TargetWindowInfoData.top) {
                        if (MousePoint.y - TargetWindowInfoData.top <= 50 && KEY_DOWN(MOUSE_MOVED)) {
                            if (MousePointBuffer.x != -25600) {
                                if (MousePoint.x <= 5) {
                                    SetTargetWindowSide(1, 0);
                                    TargetWindowInfo[0].a = 0;
                                } else if (ScreenInfo.x - MousePoint.x <= 5) {
                                    SetTargetWindowSide(1, 1);
                                    TargetWindowInfo[0].a = 1;
                                } else if (abs(MousePointBuffer.x - MousePoint.x) >= 10 ||
                                           abs(MousePointBuffer.y - MousePoint.y) >= 10) {
                                    SetTargetWindowSide(2, 0);
                                    TargetWindowInfo[0].a = -1;
                                    MousePointBuffer.x = -25600;
                                }
                            } else {
                                MousePointBuffer.x = MousePoint.x;
                                MousePointBuffer.y = MousePoint.y;
                            }
                        } else
                            MousePointBuffer.x = -25600;
                        if (TargetWindowInfo[0].a != 0 && TargetWindowInfo[0].a != 1 && TargetWindowInfoData.top <= 5) {
                            SetTargetWindowSide(1, 2);
                            TargetWindowInfo[0].a = 2;
                        }
                    } else {
                        if (TargetWindowInfo[0].a == 0)
                            SetTargetWindowSide(0, 0);
                        if (TargetWindowInfo[0].a == 1)
                            SetTargetWindowSide(0, 1);
                        if (TargetWindowInfo[0].a == 2)
                            SetTargetWindowSide(0, 2);
                    }
                }
            }
            if (TargetWindowInfo[0].a == 0)
                if (MousePoint.x <= 2)
                    SetTargetWindowSide(1, 0);
            if (TargetWindowInfo[0].a == 1)
                if (MousePoint.x >= ScreenInfo.x - 2)
                    SetTargetWindowSide(1, 1);
            if (TargetWindowInfo[0].a == 2)
                if (MousePoint.y <= 2 && MousePoint.x <= TargetWindowInfoData.right &&
                    MousePoint.x >= TargetWindowInfoData.left)
                    SetTargetWindowSide(1, 2);
        }
        if (PrintTime > 10) {
            system("cls");
            PrintTime = 0;
        }
        Sleep(200);
    }
}

long long GetTargetWindowInfo() {
    TargetWindowAddress = FindWindow(TargetClassName, TargetWindowName);
    if (TargetWindowAddress != nullptr) {
        if (::GetWindowRect(TargetWindowAddress, &TargetWindowInfoData) != 0) {
            if (TargetWindowInfoData.left == -26500 || TargetWindowInfoData.top == -25600)
                return -25600;
            else {
                TargetWindowInfo[0].x = TargetWindowInfoData.right - TargetWindowInfoData.left;
                TargetWindowInfo[0].y = TargetWindowInfoData.bottom - TargetWindowInfoData.top;
                if (TargetWindowInfoData.right <= 0) {
                    TargetWindowFlag = false;
                    TargetWindowInfo[0].a = 0;
                } else if (TargetWindowInfoData.left >= ScreenInfo.x) {
                    TargetWindowFlag = false;
                    TargetWindowInfo[0].a = 1;
                } else if ((TargetWindowInfoData.left > 5 || ScreenData.right - TargetWindowInfoData.right > 5) &&
                           TargetWindowInfoData.bottom <= 0) {
                    TargetWindowFlag = false;
                    TargetWindowInfo[0].a = 2;
                } else {
                    TargetWindowFlag = true;
                }
                return -1;
            }
        } else
            return GetLastError();
    } else {
        return GetLastError();
    }
}

void SetTargetWindowSide(int mod, int point) {
    if (mod == -1) {
        if (TargetWindowFlag)
            ::SetWindowPos(TargetWindowAddress, HWND_NOTOPMOST, TargetWindowInfoData.left, TargetWindowInfoData.top,
                           TargetWindowInfo[0].x, TargetWindowInfo[1].a, SWP_SHOWWINDOW);
        else
            ::SetWindowPos(TargetWindowAddress, HWND_NOTOPMOST, TargetWindowInfo[1].x, TargetWindowInfo[1].y,
                           TargetWindowInfo[0].x, TargetWindowInfo[1].a, SWP_SHOWWINDOW);
    } else if (mod == 2) {
        GetMouseInfoForTargetWindow();
        ::SetWindowPos(TargetWindowAddress, HWND_NOTOPMOST, MouseInfoForTargetWindow.x, MouseInfoForTargetWindow.y,
                       TargetWindowInfo[0].x, TargetWindowInfo[1].a, SWP_SHOWWINDOW);
        TargetWindowFlag = true;
    } else if (mod == 1) {
        if (point == 0)
            ::SetWindowPos(TargetWindowAddress, HWND_TOPMOST, 0, 0, TargetWindowInfo[0].x, ScreenInfo.y,
                           SWP_SHOWWINDOW);
        else if (point == 1)
            ::SetWindowPos(TargetWindowAddress, HWND_TOPMOST, ScreenInfo.x - TargetWindowInfo[0].x, 0,
                           TargetWindowInfo[0].x, ScreenInfo.y, SWP_SHOWWINDOW);
        else if (point == 2)
            ::SetWindowPos(TargetWindowAddress, HWND_TOPMOST, TargetWindowInfoData.left, 0, TargetWindowInfo[0].x,
                           TargetWindowInfo[0].y, SWP_SHOWWINDOW);
        TargetWindowFlag = true;
    } else if (mod == 0) {
        if (point == 0)
            ::SetWindowPos(TargetWindowAddress, HWND_BOTTOM, 0 - TargetWindowInfo[0].x, 0, TargetWindowInfo[0].x,
                           ScreenInfo.y, SWP_HIDEWINDOW);
        else if (point == 1)
            ::SetWindowPos(TargetWindowAddress, HWND_BOTTOM, ScreenInfo.x, 0, TargetWindowInfo[0].x,
                           ScreenInfo.y, SWP_HIDEWINDOW);
        else if (point == 2)
            ::SetWindowPos(TargetWindowAddress, HWND_BOTTOM, TargetWindowInfoData.left, 0 - TargetWindowInfo[0].y,
                           TargetWindowInfo[0].x, TargetWindowInfo[0].y, SWP_HIDEWINDOW);
        TargetWindowFlag = false;
    }
}

void SetProgramWindowShow(int mod) {
    if (mod == 0)
        ShowWindow(GetConsoleWindow(), SW_HIDE);
    else if (mod == 1)
        ShowWindow(GetConsoleWindow(), SW_SHOW);
}

void GetMouseInfoForTargetWindow() {
    MouseInfoForTargetWindow.x = MousePoint.x - TargetWindowInfoData.left;
    MouseInfoForTargetWindow.y = MousePoint.y - TargetWindowInfoData.top;
}