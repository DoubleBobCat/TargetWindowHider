//
// Created by DoubleCat.
//
#include <windows.h>

//Bar菜单键值
#define BarResetTargetWindow 17
#define BarResetWindow 12
#define BarResetSpeed 16
#define BarAutoRun 13
#define BarAbout 14
#define BarExit 15

//菜单句柄
HMENU hmenu;

//Bar Class/Windows name
LPCTSTR ClassName = TEXT("TrayBar");
LPCTSTR WindowName = TEXT("TargetWindowHider");

//Bar主体结构
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
            lstrcpy(Tray.szTip, ClassName); //此处定义Class
            Shell_NotifyIcon(NIM_ADD, &Tray);
            hmenu = CreatePopupMenu();
            ::AppendMenu(hmenu, MF_STRING, BarResetTargetWindow, "Review TargetWindow|恢复目标窗口");
            ::AppendMenu(hmenu, MF_STRING, BarResetWindow, "Reset TargetWindow|重设目标窗口");
            ::AppendMenu(hmenu, MF_STRING, BarResetSpeed, "Reset Animation|重设动画速度");
            ::AppendMenu(hmenu, MF_STRING, BarAutoRun, "Set AutoRun|设置开机自启");
            ::AppendMenu(hmenu, MF_STRING, BarAbout, "About|关于");
            ::AppendMenu(hmenu, MF_STRING, BarExit, "Exit|退出");
            // EnableMenuItem(hmenu, BarResetSpeed, MF_GRAYED);
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
                    ResetAnimationSpeedMode = true;
                    SetTargetWindowSide(3, 0);
                    ResetAnimationSpeedMode = false;
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

//创建Bar
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