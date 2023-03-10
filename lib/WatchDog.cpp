//
// Created by 19058 on 2023/3/7.
//
#include <map>
#include <string>
#include <windows.h>
#include <thread>
#include "WatchDog.h"
#include "clock.h"

std::map<int, std::string> list;
namespace WatchDog {
    int Creat_WatchDog(HWND Address, const int *checkNum, long long waitTime) {
        bool Check = true;
        WPARAM wParam;
        LPARAM lParam;
        while (Check) {
            Clock::SetClock(waitTime);
            if (*checkNum != 0) { //非正常回报
                if(*checkNum != 1){ //非冻结状态
                    SendMessage(Address, WM_DESTROY, wParam, lParam); //销毁程序
                    return *checkNum; //返回错误值
                }

            }
        }
    }
}