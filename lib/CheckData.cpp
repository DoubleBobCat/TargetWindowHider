//
// Created by DoubleCat.
//

#include "CheckData.h"
#include <string>

int isNum(const std::string &Sourse) {
    unsigned long long lenth;
    lenth = Sourse.size();
    for (unsigned long long i = 0; i < lenth;) {
        if (Sourse[i] >= '0' && Sourse[i] <= '9') {
            i++;
        } else {
            return 1;
        }
    }
    return 0;
}

int isString(const std::string &Sourse) {
    unsigned long long lenth;
    lenth = Sourse.size();
    for (unsigned long long i = 0; i < lenth;) {
        if ((Sourse[i] >= 'A' && Sourse[i] <= 'Z') || (Sourse[i] >= 'a' && Sourse[i] <= 'z')) {
            i++;
        } else {
            return 1;
        }
    }
    return 0;
}

int isPalindrome(const std::string &Sourse) {
    unsigned long long lenth;
    lenth = Sourse.size();
    for (unsigned long long i = 0; i < (lenth % 2 == 0 ? (lenth / 2) : ((lenth - 1) / 2));) {
        if (Sourse[i] == Sourse[lenth - 1 - i]) {
            i++;
        } else {
            return 1;
        }
    }
    return 0;
}

int isParentheses(const std::string &Sourse) {
    unsigned long long lenth;
    time_t L0 = 0, L1 = 0, L2 = 0, L3 = 0; // L0:( L1:{ L2=[ L3<
    lenth = Sourse.size();
    for (unsigned long long i = 0; i < lenth; i++) {
        {// 左括号
            if (Sourse[i] == '(') L0++;
            if (Sourse[i] == '{') L1++;
            if (Sourse[i] == '[') L2++;
            if (Sourse[i] == '<') L3++;
        }
        {// 右括号
            if (Sourse[i] == ')') L0--;
            if (Sourse[i] == '}') L1--;
            if (Sourse[i] == ']') L2--;
            if (Sourse[i] == '>') L3--;
        }
        if (L0 < 0 || L1 < 0 || L2 < 0 || L3 < 0) {
            return 1;
        }
    }
    if (L0 != 0 || L1 != 0 || L2 != 0 || L3 != 0) {
        return 2;
    } else {
        return 0;
    }
}

int isFunctional(const std::string &Sourse) {
    unsigned long long lenth;
    lenth = Sourse.size();
    for (unsigned long long i = 0; i < lenth;) {
        if (Sourse[i] != '+' && Sourse[i] != '-' && Sourse[i] != '*' && Sourse[i] != '/') {
            if (Sourse[i] >= '0' && Sourse[i] <= '9') {
                i++;
            } else {
                return 1;
            }
        } else {
            if (i == 0 || i == lenth - 1) {
                return 2;
            } else {
                if ((Sourse[i - 1] != '+' && Sourse[i - 1] != '-' && Sourse[i - 1] != '*' && Sourse[i - 1] != '/') &&
                    (Sourse[i + 1] != '+' && Sourse[i + 1] != '-' && Sourse[i + 1] != '*' && Sourse[i + 1] != '/')) {
                    i++;
                } else {
                    return 3;
                }
            }
        }
    }
    return 0;
}