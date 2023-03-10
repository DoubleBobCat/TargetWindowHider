//
// Created by DoubleCat.
//
// isNum return:
// 0:校验正确 1:校验失败
//
// isString return:
// 0:校验正确 1:校验失败
//
// isPalindrome return:
// 0:校验正确 1:校验失败
//
// isPalindrome return:
// 0:校验正确 1:右括号过多 2:左括号过多
//
// isFunctional return:
// 0:校验正确 1:出现意外符号 2:首尾有算数符 3:算数符左右非数
//
#ifndef MAIN_CPP_CHECKDATA_H
#define MAIN_CPP_CHECKDATA_H

#include <string>

// 校验数字
int isNum(const std::string &Sourse);

// 校验英文字符串
int isString(const std::string &Sourse);

// 校验回文
int isPalindrome(const std::string &Sourse);

// 校验括号匹配
int isParentheses(const std::string &Sourse);

// 校验函数式
int isFunctional(const std::string &Sourse);

#endif //MAIN_CPP_CHECKDATA_H
