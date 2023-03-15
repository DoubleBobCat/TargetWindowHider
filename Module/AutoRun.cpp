//
// Created by DoubleCat.
//
#include <string>

char *StringToChar(const std::string &object) {
    char *result = (char *) object.data();
    return result;
}

void AutoRun(const std::string &RegName) { //admin
    std::string temp = R"(REG ADD HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Run /v )";
    const std::string &autorunName = RegName;
    std::string path = __argv[0];
    std::string cmd = temp + autorunName + " /t REG_SZ /d " + path + " /f";
    system(StringToChar(cmd));
}