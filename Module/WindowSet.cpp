//
// Created by DoubleCat.
//
#include <windows.h>

//定义数据结构体
struct WindowInfo {
    int x, y; //窗口信息
    int status; //状态代码
};

WindowInfo ToWhere(){

}

int SetTargetWindowSide(int mod, int point, HWND TargetWindowAddress, WindowInfo &TargetWindowInfo,WindowInfo &ScreenInfo,bool &ProgramWindowShow) {
    if (mod == 3) {
        ::SetWindowPos(TargetWindowAddress, HWND_NOTOPMOST, (ScreenInfo.x - TargetWindowInfo[0].x) / 2, (ScreenInfo.y - TargetWindowInfo[1].y) / 2,
                       TargetWindowInfo[0].x, TargetWindowInfo[1].y, SWP_SHOWWINDOW);
        ProgramWindowShow = true;
        GetTargetWindowInfo();
    } else if (mod == 2) {
        ::SetWindowPos(TargetWindowAddress, HWND_NOTOPMOST, TargetWindowInfoData.left, TargetWindowInfoData.top, TargetWindowInfo[0].x,
                       TargetWindowInfo[1].y, SWP_SHOWWINDOW);
        ProgramWindowShow = true;
    } else if (mod == 1) {
        if (point == 0)
            TargetWindowAnimation(1, 0, 0, TargetWindowInfo[0].x, ScreenInfo.y);
        else if (point == 1)
            TargetWindowAnimation(1, ScreenInfo.x - TargetWindowInfo[0].x, 0, TargetWindowInfo[0].x, ScreenInfo.y);
        else if (point == 2)
            TargetWindowAnimation(1, TargetWindowInfoData.left, 0, TargetWindowInfo[0].x, TargetWindowInfo[0].y);
        ProgramWindowShow = true;
    } else if (mod == 0) {
        if (point == 0)
            TargetWindowAnimation(0, 0 - TargetWindowInfo[0].x, 0, TargetWindowInfo[0].x, ScreenInfo.y);
        else if (point == 1)
            TargetWindowAnimation(0, ScreenInfo.x, 0, TargetWindowInfo[0].x, ScreenInfo.y);
        else if (point == 2)
            TargetWindowAnimation(0, TargetWindowInfoData.left, 0 - TargetWindowInfo[0].y, TargetWindowInfo[0].x, TargetWindowInfo[0].y);
        ProgramWindowShow = false;
    }
    TargetWindowInfo[1].flag = TargetWindowInfo[0].flag;
}

// Hide:mod=0 Show:mod=1
// WindowAddress存储窗口句柄
// ProgramWindowShow传递窗口状态
void SetWindowShow(int mod, HWND WindowAddress, bool &ProgramWindowShow) {
    if (mod == 0) {
        ShowWindow(WindowAddress, SW_HIDE);
        ProgramWindowShow = false;
    } else if (mod == 1) {
        ShowWindow(WindowAddress, SW_SHOW);
        ProgramWindowShow = true;
    }
}