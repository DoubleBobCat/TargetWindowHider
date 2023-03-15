//
// Created by DoubleCat.
//
#include <windows.h>

//定义数据结构体
struct WindowInfo {
    int x, y; //窗口信息
    int status; //状态代码
};

WindowInfo ToWhere() {

}

// 动态移动窗口
void TargetWindowAnimation(int mod, int left, int top, int x, int y,int AnimationSpeed, HWND TargetWindowAddress, RECT TargetWindowInfoData) {
    int left_, top_;
    left_ = (left - TargetWindowInfoData.left) / 5;
    top_ = (top - TargetWindowInfoData.top) / 5;
    if (AnimationSpeed == 0) {
        if (mod == 1) ::SetWindowPos(TargetWindowAddress, HWND_TOPMOST, left, top, x, y, SWP_SHOWWINDOW);
        if (mod == 0) ::SetWindowPos(TargetWindowAddress, HWND_BOTTOM, left, top, x, y, SWP_HIDEWINDOW);
    } else {
        left = TargetWindowInfoData.left;
        top = TargetWindowInfoData.top;
        if (mod == 1) {
            ::SetWindowPos(TargetWindowAddress, HWND_TOPMOST, TargetWindowInfoData.left, TargetWindowInfoData.top, x, y, SWP_SHOWWINDOW);
            for (size_t i = 0; i < 5; i++) {
                left += left_;
                top += top_;
                ::MoveWindow(TargetWindowAddress, left, top, x, y, SWP_SHOWWINDOW);
                Sleep(800 / 5);
            }
        }
        if (mod == 0) {
            ::SetWindowPos(TargetWindowAddress, HWND_BOTTOM, TargetWindowInfoData.left, TargetWindowInfoData.top, x, y, SWP_HIDEWINDOW);
            for (size_t i = 0; i < 5; i++) {
                left += left_;
                top += top_;
                ::MoveWindow(TargetWindowAddress, left, top, x, y, SWP_HIDEWINDOW);
                Sleep(800 / 5);
            }
        }
    }
}

// 按照规则移动窗口
// mod=1:还原窗口 mod=2:还原窗口 mod=3:还原窗口 mod=4:还原窗口
// point=0:左
int SetTargetWindowSide(int mod, int point,int AnimationSpeed, HWND TargetWindowAddress, WindowInfo *TargetWindowInfo, WindowInfo &ScreenInfo, bool &ProgramWindowShow,
                        RECT &TargetWindowInfoData) {
    if (mod == 3) {
        ::SetWindowPos(TargetWindowAddress, HWND_NOTOPMOST, (ScreenInfo.x - TargetWindowInfo[0].x) / 2, (ScreenInfo.y - TargetWindowInfo[1].y) / 2,
                       TargetWindowInfo[0].x, TargetWindowInfo[1].y, SWP_SHOWWINDOW);
        ProgramWindowShow = true;
    } else if (mod == 2) {
        ::SetWindowPos(TargetWindowAddress, HWND_NOTOPMOST, TargetWindowInfoData.left, TargetWindowInfoData.top, TargetWindowInfo[0].x,
                       TargetWindowInfo[1].y, SWP_SHOWWINDOW);
        ProgramWindowShow = true;
    } else if (mod == 1) {
        if (point == 0)
            TargetWindowAnimation(1, 0, 0, TargetWindowInfo[0].x, ScreenInfo.y,AnimationSpeed,TargetWindowAddress,TargetWindowInfoData);
        else if (point == 1)
            TargetWindowAnimation(1, ScreenInfo.x - TargetWindowInfo[0].x, 0, TargetWindowInfo[0].x, ScreenInfo.y,AnimationSpeed,TargetWindowAddress,TargetWindowInfoData);
        else if (point == 2)
            TargetWindowAnimation(1, TargetWindowInfoData.left, 0, TargetWindowInfo[0].x, TargetWindowInfo[0].y,AnimationSpeed,TargetWindowAddress,TargetWindowInfoData);
        ProgramWindowShow = true;
    } else if (mod == 0) {
        if (point == 0)
            TargetWindowAnimation(0, 0 - TargetWindowInfo[0].x, 0, TargetWindowInfo[0].x, ScreenInfo.y,AnimationSpeed,TargetWindowAddress,TargetWindowInfoData);
        else if (point == 1)
            TargetWindowAnimation(0, ScreenInfo.x, 0, TargetWindowInfo[0].x, ScreenInfo.y,AnimationSpeed,TargetWindowAddress,TargetWindowInfoData);
        else if (point == 2)
            TargetWindowAnimation(0, TargetWindowInfoData.left, 0 - TargetWindowInfo[0].y, TargetWindowInfo[0].x, TargetWindowInfo[0].y,AnimationSpeed,TargetWindowAddress,TargetWindowInfoData);
        ProgramWindowShow = false;
    }
    TargetWindowInfo[1].status = TargetWindowInfo[0].status;
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