#include <iostream>
#include <windows.h>
#include <cstring>
#include <string>
#include <fstream>
#include <thread>

struct WindowInfo {
    int x, y, flag;
} TargetWindowInfo[2], ScreenInfo;

void GetTargetWindowInfo();

void GetMouseInfo();

void SetTargetWindowSide(int mod, int point);

void SetProgramWindowShow(int mod);

void GetConfigFile();

void StepFunction();

HWND TargetWindowAddress;
RECT TargetWindowInfoData, ScreenData;
POINT MousePoint, MousePointBuffer;
size_t PrintTime = 0, GetConfigFile_T = 0;
bool TargetWindowShowFlag = true, MouseInTargetWindow = false, MouseInShowSide = false;
std::string TargetClassNameData, TargetWindowNameData;
int FindWindowMode;
LPCSTR TargetClassName, TargetWindowName;
std::thread StepFunction_P(StepFunction); //NOLINT

int main() {
    system("chcp 65001");//设置程序语言为中文
    SetProgramWindowShow(0);
    SetWindowText(GetConsoleWindow(), "WindowHider");
    std::cout << "程序开始运行……" << std::endl;
    GetConfigFile();
    if (!::SystemParametersInfoA(SPI_GETWORKAREA, 0, &ScreenData, 0)) {
        SetProgramWindowShow(1);
        std::cout << "无法获取屏幕信息，请联系开发者。Error_code: " << GetLastError() << std::endl << "程序将在5s后自动关闭";
        Sleep(5000);
        exit((int) GetLastError());
    }
    ScreenInfo.x = ScreenData.right - ScreenData.left;
    ScreenInfo.y = ScreenData.bottom - ScreenData.top;
    MousePointBuffer.x = -25600;
    GetTargetWindowInfo();
    if (TargetWindowInfo[0].y <= (int) ((double) ScreenInfo.y * 0.8))
        TargetWindowInfo[1].y = TargetWindowInfo[0].y;
    TargetWindowInfo[1].flag = TargetWindowInfo[0].flag;
    StepFunction_P.join();
    return 0;
}

void GetTargetWindowInfo() {
    if (FindWindowMode == 0)
        TargetWindowAddress = FindWindow(nullptr, TargetWindowName);
    else if (FindWindowMode == 1)
        TargetWindowAddress = FindWindow(TargetClassName, nullptr);
    else
        TargetWindowAddress = FindWindow(TargetClassName, TargetWindowName);
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
                } else if (TargetWindowInfoData.left <= 5) {
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
    Sleep(10);
}

void GetMouseInfo() {
    if (GetCursorPos(&MousePoint) == 0) {
        SetProgramWindowShow(1);
        std::cout << "无法获取鼠标信息，请联系开发者。Error_code: " << GetLastError() << std::endl << "程序将在5s后自动关闭";
        Sleep(5000);
        exit(102);
    } else {
        MouseInTargetWindow = MousePoint.x <= TargetWindowInfoData.right && MousePoint.x >= TargetWindowInfoData.left;
        MouseInTargetWindow = MouseInTargetWindow && MousePoint.y >= TargetWindowInfoData.top && MousePoint.y <= TargetWindowInfoData.bottom;
    }
}

void SetTargetWindowSide(int mod, int point) {
    if (mod == 2) {
        ::SetWindowPos(TargetWindowAddress, HWND_NOTOPMOST, TargetWindowInfoData.left, TargetWindowInfoData.top, TargetWindowInfo[0].x,
                       TargetWindowInfo[1].y, SWP_SHOWWINDOW);
        TargetWindowShowFlag = true;
    } else if (mod == 1) {
        if (point == 0)
            ::SetWindowPos(TargetWindowAddress, HWND_TOPMOST, 0, 0, TargetWindowInfo[0].x, ScreenInfo.y, SWP_SHOWWINDOW);
        else if (point == 1)
            ::SetWindowPos(TargetWindowAddress, HWND_TOPMOST, ScreenInfo.x - TargetWindowInfo[0].x, 0, TargetWindowInfo[0].x, ScreenInfo.y, SWP_SHOWWINDOW);
        else if (point == 2)
            ::SetWindowPos(TargetWindowAddress, HWND_TOPMOST, TargetWindowInfoData.left, 0, TargetWindowInfo[0].x, TargetWindowInfo[0].y, SWP_SHOWWINDOW);
        TargetWindowShowFlag = true;
    } else if (mod == 0) {
        if (point == 0)
            ::SetWindowPos(TargetWindowAddress, HWND_BOTTOM, 0 - TargetWindowInfo[0].x, 0, TargetWindowInfo[0].x, ScreenInfo.y, SWP_HIDEWINDOW);
        else if (point == 1)
            ::SetWindowPos(TargetWindowAddress, HWND_BOTTOM, ScreenInfo.x, 0, TargetWindowInfo[0].x, ScreenInfo.y, SWP_HIDEWINDOW);
        else if (point == 2)
            ::SetWindowPos(TargetWindowAddress, HWND_BOTTOM, TargetWindowInfoData.left, 0 - TargetWindowInfo[0].y, TargetWindowInfo[0].x,
                           TargetWindowInfo[0].y, SWP_HIDEWINDOW);
        TargetWindowShowFlag = false;
    }
    TargetWindowInfo[1].flag = TargetWindowInfo[0].flag;
}

void SetProgramWindowShow(int mod) {
    if (mod == 0)
        ShowWindow(GetConsoleWindow(), SW_HIDE);
    else if (mod == 1)
        ShowWindow(GetConsoleWindow(), SW_SHOW);
}

void GetConfigFile() { //NOLINT
    std::fstream ConfigFile;
    ConfigFile.open("config.json", std::ios::in);
    if (!ConfigFile.is_open()) {
        SetProgramWindowShow(1);
        ConfigFile.open("config.json", std::ios::out);
        std::cout << "未找到config.json文件，正在创建……" << std::endl << "请分为两行输入目标程序类名、窗口名，未知项请输入nullptr，两者至少需要填写一项："
                  << std::endl;
        std::cin >> TargetClassNameData >> TargetWindowNameData;
        ConfigFile << TargetClassNameData << std::endl << TargetWindowNameData;
    }
    ConfigFile >> TargetClassNameData >> TargetWindowNameData;
    if (TargetClassNameData.find("nullptr") != std::string::npos &&
        TargetWindowNameData.find("nullptr") != std::string::npos) {
        std::cout << "类名、窗口名均为nullptr，请重新输入。" << std::endl;
        ConfigFile.close();
        DeleteFile("config.json");
        if (GetConfigFile_T < 4) {
            GetConfigFile_T++;
            GetConfigFile();
        } else {
            std::cout << "5次了啊5次你干嘛啊！！！" << std::endl;
            Sleep(5000);
            exit(250);
        }
    }
    ConfigFile.close();
    TargetClassName = strdup(TargetClassNameData.c_str());
    TargetWindowName = strdup(TargetWindowNameData.c_str());
    if (TargetClassNameData.find("nullptr") != std::string::npos)
        FindWindowMode = 0;
    else if (TargetWindowNameData.find("nullptr") != std::string::npos)
        FindWindowMode = 1;
    else
        FindWindowMode = 2;
    SetProgramWindowShow(0);
}

void StepFunction() {
    while (true) { //NOLINT
        GetTargetWindowInfo();
        GetMouseInfo();
        if (TargetWindowShowFlag) {
            if (TargetWindowInfo[0].flag != -1 && (!MouseInTargetWindow || !MouseInShowSide)) { SetTargetWindowSide(0, TargetWindowInfo[0].flag); }
            if ((TargetWindowInfo[1].flag == 0 || TargetWindowInfo[1].flag == 1 || TargetWindowInfo[1].flag == 2) && TargetWindowInfo[0].flag == -1) {
                SetTargetWindowSide(2, 0);
                TargetWindowInfo[1].flag = -1;
            }
        } else {
            MouseInShowSide = TargetWindowInfo[0].flag == 2 && MousePoint.x >= TargetWindowInfoData.left && MousePoint.x <= TargetWindowInfoData.right &&
                              MousePoint.y < 5;
            MouseInShowSide = MouseInShowSide || TargetWindowInfo[0].flag == 0 && MousePoint.x <= 5;
            MouseInShowSide = MouseInShowSide || TargetWindowInfo[0].flag == 1 && MousePoint.x >= (ScreenInfo.x - 5);
            if (MouseInShowSide) { SetTargetWindowSide(1, TargetWindowInfo[0].flag); }
        }
        if (TargetWindowShowFlag && TargetWindowInfo[0].y <= (int) ((double) ScreenInfo.y * 0.8))
            TargetWindowInfo[1].y = TargetWindowInfo[0].y;
        if (TargetWindowShowFlag && TargetWindowInfo[0].flag == -1 && TargetWindowInfo[0].y != TargetWindowInfo[1].y)
            SetTargetWindowSide(2, 0);
        if (PrintTime > 13) {
            system("cls");
            PrintTime = 0;
        }
        Sleep(200);
    }
}