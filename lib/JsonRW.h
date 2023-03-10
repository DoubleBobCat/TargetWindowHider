//
// Created by DoubleCat.
//

#ifndef MAIN_CPP_JSONRW_H
#define MAIN_CPP_JSONRW_H

#include<iostream>
#include<map>

namespace JsonRW {
    int readFileJson(const std::string &JsonAddress, std::map<int, std::tuple<std::string, std::string, int>> &DataBase);

    int writeFileJson(const std::string &JsonAddress, std::map<int, std::tuple<std::string, std::string, int>> &DataBase);
}
#endif //MAIN_CPP_JSONRW_H
