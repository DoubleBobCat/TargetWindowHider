#include <iostream>
#include <windows.h>
#include <string>
#include <fstream>
#include <thread>
#include <shellapi.h>

#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0)
#define BarResetTargetWindow 17
#define BarResetWindow 12
#define BarResetSpeed 16
#define BarAutoRun 13
#define BarAbout 14
#define BarExit 15

struct WindowInfo {
    int x, y, flag;
} TargetWindowInfo[2], ScreenInfo;

void GetTargetWindowInfo();

void GetMouseInfo();

void SetTargetWindowSide(int mod, int point);

void SetProgramWindowShow(int mod);

void GetConfigFile();

void StepFunction();

void TrayBar();

void AutoRun(const std::string &RegName);

void TargetWindowAnimation(int mod, int left, int top, int x, int y);

HWND TargetWindowAddress;
RECT ProgramWindowInfoData, TargetWindowInfoData, ScreenData;
POINT MousePoint, MousePointBuffer;
size_t PrintTime = 0;
bool TargetWindowShowFlag = true, MouseInTargetWindow = false, MouseInShowSide = false, ProgramWindowShow = false, ExitMod = false;
bool ResetTargetWindowMode = false, ResetAnimationSpeedMode = false;
std::string TargetClassNameData, TargetWindowNameData, AnimationSpeedData;
int FindWindowMode, AnimationSpeed;
LPCSTR TargetClassName, TargetWindowName;
std::thread StepFunction_thread(StepFunction), TrayBar_thread(TrayBar), GetTargetWindowInfo_thread(GetTargetWindowInfo); //NOLINT
LPCTSTR ClassName = TEXT("TrayBar");
LPCTSTR WindowName = TEXT("TargetWindowHider");
HMENU hmenu;

int main() {
    ::SetWindowPos(GetConsoleWindow(), nullptr, 160, 90, 480, 270, SWP_HIDEWINDOW);
    SetWindowText(GetConsoleWindow(), "TargetWindowHider");
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
    TrayBar_thread.detach();
    GetTargetWindowInfo_thread.detach();
    StepFunction_thread.join();
    return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    NOTIFYICONDATA Tray;
    UINT RegTaskBar;
    int BarExitCode;
    RegTaskBar = RegisterWindowMessage(TEXT("TaskbarCreated"));
    switch (message) {
        case WM_CREATE:
            Tray.cbSize = sizeof(Tray);
            Tray.hWnd = hwnd;
            Tray.uID = 0;
            Tray.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
            Tray.uCallbackMessage = WM_USER;
            Tray.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
            lstrcpy(Tray.szTip, ClassName);
            Shell_NotifyIcon(NIM_ADD, &Tray);
            hmenu = CreatePopupMenu();
            ::AppendMenu(hmenu, MF_STRING, BarResetTargetWindow, "Review TargetWindow|恢复目标窗口");
            ::AppendMenu(hmenu, MF_STRING, BarResetWindow, "Reset TargetWindow|重设目标窗口");
            ::AppendMenu(hmenu, MF_STRING, BarResetSpeed, "Reset Animation|重设动画速度");
            ::AppendMenu(hmenu, MF_STRING, BarAutoRun, "Set AutoRun|设置开机自启");
            ::AppendMenu(hmenu, MF_STRING, BarAbout, "About|关于");
            ::AppendMenu(hmenu, MF_STRING, BarExit, "Exit|退出");
            EnableMenuItem(hmenu, BarResetSpeed, MF_GRAYED);
            break;
        case WM_USER:
            if (lParam == WM_LBUTTONDOWN) {
                if (ProgramWindowShow) SetProgramWindowShow(0);
                else SetProgramWindowShow(1);
            }
            if (lParam == WM_RBUTTONDOWN) {
                GetCursorPos(&MousePoint);
                ::SetForegroundWindow(hwnd);
                BarExitCode = ::TrackPopupMenu(hmenu, TPM_RETURNCMD, MousePoint.x, MousePoint.y, 0, hwnd, nullptr);
                if (BarExitCode == BarResetTargetWindow) {
                    SetTargetWindowSide(3, 0);
                }
                if (BarExitCode == BarResetWindow) {
                    system("cls");
                    SetProgramWindowShow(1);
                    std::cout << "正在关闭进程……" << std::endl << "即将重置……" << std::endl;
                    ResetTargetWindowMode = true;
                    SetTargetWindowSide(3, 0);
                    DeleteFile("config.json");
                    TargetClassNameData = "";
                    TargetWindowNameData = "";
                    GetConfigFile();
                    ResetTargetWindowMode = false;
                    SetProgramWindowShow(0);
                }
                if (BarExitCode == BarResetSpeed) {
                    system("cls");
                    SetProgramWindowShow(1);
                    std::cout << "正在关闭进程……" << std::endl << "即将重置……" << std::endl;
                    ResetAnimationSpeedMode = true;
                    SetTargetWindowSide(3, 0);
                    DeleteFile("config.json");
                    AnimationSpeedData = "";
                    GetConfigFile();
                    ResetAnimationSpeedMode = false;
                    SetProgramWindowShow(0);
                }
                if (BarExitCode == BarAutoRun)
                    AutoRun("TargetWindowHider");
                if (BarExitCode == BarAbout)
                    MessageBox(hwnd, TEXT("TargetWindowHider V1.1.8\nAuthor: DoubleCat\nWebsite: blog.doublecat.cn"), ClassName, MB_OK);
                if (BarExitCode == BarExit) {
                    SendMessage(hwnd, WM_DESTROY, wParam, lParam);
                    ExitMod = true;
                    exit(0);
                }
                if (BarExitCode == 0)
                    PostMessage(hwnd, WM_LBUTTONDOWN, 0, 0);
            }
            break;
        case WM_DESTROY:
            Shell_NotifyIcon(NIM_DELETE, &Tray);
            PostQuitMessage(0);
            break;
        default:
            if (message == RegTaskBar)
                SendMessage(hwnd, WM_CREATE, wParam, lParam);
            break;
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}

char *StringToChar(const std::string &object) {
    char *result = (char *) object.data();
    return result;
}

void TrayBar() {
    HWND hwnd;
    MSG msg;
    WNDCLASS wndclass;
    HWND handle = FindWindow(nullptr, WindowName);
    if (handle != nullptr) {
        MessageBox(nullptr, TEXT("Application is already running"), ClassName, MB_ICONERROR);
        exit(555);
    }
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = nullptr;
    wndclass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName = nullptr;
    wndclass.lpszClassName = ClassName;

    if (!RegisterClass(&wndclass)) {
        MessageBox(nullptr, TEXT("This program requires Windows NT!"), ClassName, MB_ICONERROR);
        exit(2008);
    }
    hwnd = CreateWindowEx(WS_EX_TOOLWINDOW,
                          ClassName, WindowName,
                          WS_POPUP,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          nullptr, nullptr, nullptr, nullptr);

    ShowWindow(hwnd, 0);
    UpdateWindow(hwnd);
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void AutoRun(const std::string &RegName) { //admin
    std::string temp = R"(REG ADD HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Run /v )";
    const std::string &autorunName = RegName;
    std::string path = __argv[0];
    std::string cmd = temp + autorunName + " /t REG_SZ /d " + path + " /f";
    system(StringToChar(cmd));
}

void GetTargetWindowInfo() {
    while (!ExitMod) {
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
    if (mod == 3) {
        ::SetWindowPos(TargetWindowAddress, HWND_NOTOPMOST, (ScreenInfo.x - TargetWindowInfo[0].x) / 2, (ScreenInfo.y - TargetWindowInfo[1].y) / 2,
                       TargetWindowInfo[0].x, TargetWindowInfo[1].y, SWP_SHOWWINDOW);
        TargetWindowShowFlag = true;
    } else if (mod == 2) {
        ::SetWindowPos(TargetWindowAddress, HWND_NOTOPMOST, TargetWindowInfoData.left, TargetWindowInfoData.top, TargetWindowInfo[0].x,
                       TargetWindowInfo[1].y, SWP_SHOWWINDOW);
        TargetWindowShowFlag = true;
    } else if (mod == 1) {
        if (point == 0)
            TargetWindowAnimation(1, 0, 0, TargetWindowInfo[0].x, ScreenInfo.y);
        else if (point == 1)
            TargetWindowAnimation(1, ScreenInfo.x - TargetWindowInfo[0].x, 0, TargetWindowInfo[0].x, ScreenInfo.y);
        else if (point == 2)
            TargetWindowAnimation(1, TargetWindowInfoData.left, 0, TargetWindowInfo[0].x, TargetWindowInfo[0].y);
        TargetWindowShowFlag = true;
    } else if (mod == 0) {
        if (point == 0)
            TargetWindowAnimation(0, 0 - TargetWindowInfo[0].x, 0, TargetWindowInfo[0].x, ScreenInfo.y);
        else if (point == 1)
            TargetWindowAnimation(0, ScreenInfo.x, 0, TargetWindowInfo[0].x, ScreenInfo.y);
        else if (point == 2)
            TargetWindowAnimation(0, TargetWindowInfoData.left, 0 - TargetWindowInfo[0].y, TargetWindowInfo[0].x, TargetWindowInfo[0].y);
        TargetWindowShowFlag = false;
    }
    TargetWindowInfo[1].flag = TargetWindowInfo[0].flag;
}

void SetProgramWindowShow(int mod) {
    if (mod == 0) {
        ShowWindow(GetConsoleWindow(), SW_HIDE);
        ProgramWindowShow = false;
    } else if (mod == 1) {
        ShowWindow(GetConsoleWindow(), SW_SHOW);
        ProgramWindowShow = true;
    }
}

void GetConfigFile() {
    bool AnimationSpeedCheck = false;
    if (!ResetTargetWindowMode && !ResetAnimationSpeedMode) {
        std::fstream ConfigFile;
        ConfigFile.open("config.json", std::ios::in);
        if (!ConfigFile.is_open()) {
            SetProgramWindowShow(1);
            ConfigFile.open("config.json", std::ios::out);
            std::cout << "未找到config.json文件，正在创建……" << std::endl << "请分为三行分别输入目标程序的类名、窗口名以及动画速度" << std::endl
                      << "类名和窗口名若不指定则填\"nullptr\"，类名和窗口名最少指定一个" << std::endl << "动画速度的单位为毫秒(ms)(无动画为0)" << std::endl;
            std::cin >> TargetClassNameData >> TargetWindowNameData >> AnimationSpeedData;
        } else {
            ConfigFile >> TargetClassNameData >> TargetWindowNameData >> AnimationSpeedData;
            if (TargetClassNameData.empty()) {
                std::cout << "配置文件内的窗口类名为空，请重新输入！" << std::endl;
                std::cin >> TargetClassNameData;
            }
            if (TargetWindowNameData.empty()) {
                std::cout << "配置文件内的窗口名为空，请重新输入！" << std::endl;
                std::cin >> TargetWindowNameData;
            }
            if (AnimationSpeedData.empty()) {
                std::cout << "配置文件内的动画速度为空，请重新输入！" << std::endl;
                std::cin >> AnimationSpeedData;
            }
        }
        ConfigFile.close();
    } else {
        system("cls");
        SetProgramWindowShow(1);
        if (TargetClassNameData.empty() && TargetWindowNameData.empty()) {
            std::cout << "请分为两行分别输入目标程序的类名、窗口名" << std::endl << "类名和窗口名若不指定则填\"nullptr\"，类名和窗口名最少指定一个" << std::endl;
            std::cin >> TargetClassNameData >> TargetWindowNameData;
        } else if (AnimationSpeedData.empty()) {
            std::cout << "请输入动画速度(无动画为0)" << std::endl;
            std::cin >> AnimationSpeedData;
        }
    }
    while (TargetClassNameData == "nullptr" && TargetWindowNameData == "nullptr") {
        TargetClassNameData = "";
        TargetWindowNameData = "";
        std::cout << "类名、窗口名均为nullptr，请重新输入:" << std::endl;
        std::cin >> TargetClassNameData >> TargetWindowNameData;
    }
    while (!AnimationSpeedCheck) {
        AnimationSpeedCheck = true;
        int AnimationSpeedNum = (int) AnimationSpeedData.size();
        for (size_t i = 0; i < AnimationSpeedNum; i++) {
            if (!isdigit(AnimationSpeedData[i])) {
                std::cout << "动画时间请不要输入数字以外的字符" << std::endl;
                i = AnimationSpeedNum;
                AnimationSpeedCheck = false;
            }
        }
        if (AnimationSpeedCheck) {
            if (std::stoi(AnimationSpeedData) <= 5000 && std::stoi(AnimationSpeedData) >= 0) {}
            else {
                std::cout << "动画时间不能超过5000ms(即5s)，不能小于0ms" << std::endl;
                AnimationSpeedCheck = false;
            }
        }
        if (!AnimationSpeedCheck) {
            std::cout << "请重新输入动画时间(单位:ms)(无动画为0)" << std::endl;
            std::cin >> AnimationSpeedData;
        }
    }
    TargetClassName = strdup(TargetClassNameData.c_str());
    TargetWindowName = strdup(TargetWindowNameData.c_str());
    AnimationSpeed = std::stoi(AnimationSpeedData);
    AnimationSpeed = 0;
    SetProgramWindowShow(0);
    system("cls");
    std::fstream ConfigFile;
    DeleteFile("config.json");
    ConfigFile.open("config.json", std::ios::out);
    ConfigFile << TargetClassNameData << std::endl << TargetWindowNameData << std::endl << AnimationSpeedData;
    ConfigFile.close();
    if (TargetClassNameData == "nullptr")
        FindWindowMode = 0;
    else if (TargetWindowNameData == "nullptr")
        FindWindowMode = 1;
    else
        FindWindowMode = 2;
    SetProgramWindowShow(0);
}

void StepFunction() {
    while (!ExitMod) {
        while (ResetTargetWindowMode) {}
        while (ResetAnimationSpeedMode) {}
        if (TargetWindowShowFlag) {
            if (TargetWindowInfo[0].flag != -1 && (!MouseInTargetWindow || !MouseInShowSide)) { SetTargetWindowSide(0, TargetWindowInfo[0].flag); }
            if ((TargetWindowInfo[1].flag == 0 || TargetWindowInfo[1].flag == 1 || TargetWindowInfo[1].flag == 2) && TargetWindowInfo[0].flag == -1) {
                while (!KEY_DOWN(MOUSE_MOVED)) {
                    SetTargetWindowSide(2, 0);
                    TargetWindowInfo[1].flag = -1;
                }
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
        if (PrintTime > 13) {
            system("cls");
            PrintTime = 0;
        }
        Sleep(200);
    }
    if (!TargetWindowShowFlag)
        SetTargetWindowSide(3, 0);
    exit(0);
}

void TargetWindowAnimation(int mod, int left, int top, int x, int y) {
    int left_, top_;
    left_ = (left - TargetWindowInfoData.left) / 5;
    top_ = (top - TargetWindowInfoData.top) / 5;
    if (AnimationSpeed == 0) {
        if (mod == 1) ::SetWindowPos(TargetWindowAddress, HWND_TOPMOST, left, top, x, y, SWP_SHOWWINDOW);
        if (mod == 0) ::SetWindowPos(TargetWindowAddress, HWND_BOTTOM, left, top, x, y, SWP_HIDEWINDOW);
    } else {
        left = TargetWindowInfoData.left;
        top = TargetWindowInfoData.top;
        for (size_t i = 0; i < 5; i++) {
            left += left_;
            top += top_;
            if (mod == 1) ::SetWindowPos(TargetWindowAddress, HWND_TOPMOST, left, top, x, y, SWP_SHOWWINDOW);
            if (mod == 0) ::SetWindowPos(TargetWindowAddress, HWND_BOTTOM, left, top, x, y, SWP_HIDEWINDOW);
            Sleep(800 / 5);
        }
    }
}