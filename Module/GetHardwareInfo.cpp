//
// Created by DoubleCat.
//
#include <iostream>
#include <vector>
#include <Windows.h>

HMONITOR MonitorFromIndex(int i) {
    return nullptr;
}

// 获取屏幕RECT
RECT GetScreenRect(int screen) {
    DISPLAY_DEVICEW device;
    device.cb = sizeof(device);
    BOOL result = EnumDisplayDevicesW(NULL, screen, &device, 0);
    DEVMODEW device_mode;
    device_mode.dmSize = sizeof(device_mode);
    device_mode.dmDriverExtra = 0;
    result = EnumDisplaySettingsExW(device.DeviceName, ENUM_CURRENT_SETTINGS, &device_mode, 0);
    int x = device_mode.dmPosition.x;
    int y = device_mode.dmPosition.y;
    int width = device_mode.dmPelsWidth;
    int height = device_mode.dmPelsHeight;
    return {x, y, x + width, y + height};
}

// 获取多显示器大小
int GetScreenSize(std::vector<RECT> *v) {
    int numbers = GetSystemMetrics(SM_CMONITORS);
    for (int i = 0; i < numbers; ++i) {
        v->push_back(GetScreenRect(i));
    }
    return 0;
}

// 获取主显示器大小
int GetMainScreenSize(RECT *data){
    ::SystemParametersInfoA(SPI_GETWORKAREA, 0, data, 0);
}

// 获取鼠标位置
int GetMouseInfo(POINT *MousePoint) {
    if (GetCursorPos(MousePoint) == 0) {
        return 1;
    } else {
        return 0;
    }
}