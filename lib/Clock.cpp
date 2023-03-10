//
// Created by 19058 on 2023/3/7.
//
#include "Clock.h"
#include <sys/time.h>
#include <ctime>
#include <chrono>

struct Time_base {
    long long m, s;
} Time[2];

long long getDeltaTime() {
    std::chrono::system_clock::duration d = std::chrono::system_clock::now().time_since_epoch();
    std::chrono::minutes min = std::chrono::duration_cast<std::chrono::minutes>(d);
    std::chrono::seconds sec = std::chrono::duration_cast<std::chrono::seconds>(d);
    Time[0].m = Time[1].m;
    Time[0].s = Time[1].s;
    Time[1].m = min.count();
    Time[1].s = sec.count();
    long long deltaTime = 0;
    if (Time[0].m > Time[1].m) {
        deltaTime += (60 - Time[0].m + Time[1].m) * 60;
    } else {
        deltaTime += (Time[1].m - Time[0].m) * 60;
    }
    if (Time[0].s > Time[1].s) {
        deltaTime += 60 - Time[0].s + Time[1].s;
    } else {
        deltaTime += Time[1].s - Time[0].s;
    }
    return deltaTime;
}

int SetClock(long long checkNum) {
    checkNum += 1;
}