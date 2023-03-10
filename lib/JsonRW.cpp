//
// Created by DoubleCat on 2023/3/7.
//
// int readFileJson return
// 0:解析成功 1:文件加载失败 2:json解析失败
//
// int writeFileJson return
// 0:写入成功
#include "JsonRW.h"
#include "Json.h"
#include <iostream>
#include <fstream>
#include <map>

int readFileJson(const std::string &JsonAddress, std::map<int, std::tuple<std::string, std::string, int>> &DataBase) {
    Json::Reader reader;
    Json::Value root;
    // 读入json
    std::ifstream JsonData(JsonAddress, std::ios::binary);
    if (!JsonData.is_open()) {
        return 1;
    }
    // 解析json
    bool parsingSuccessful = reader.parse(JsonData, root);
    if (!parsingSuccessful) {
        return 2;
    }
    // 遍历Json数组
    time_t ID = 0;
    for (Json::Value::iterator it = root.begin(); it != root.end(); ++it) {
        ID++;
        // 获取当前元素
        Json::Value item = *it;
        // 获取"TargetClassNameData"和"TargetWindowNameData"和"AnimationSpeedData"字段
        std::string TargetClassNameData = item["TargetClassName"].asString();
        std::string TargetWindowNameData = item["TargetWindowName"].asString();
        int AnimationSpeedData = item["AnimationSpeed"].asInt();
        // 存入数据
        auto TempData = std::make_tuple(TargetClassNameData, TargetWindowNameData, AnimationSpeedData);
        DataBase.insert(std::make_pair(ID, TempData));
    }
    JsonData.close();
    return 0;
}

int writeFileJson(const std::string &JsonAddress, std::map<int, std::tuple<std::string, std::string, int>> &DataBase) {
    Json::Value root;
    // 读入json
    std::ofstream JsonData(JsonAddress, std::ios::binary);
    // 遍历Json数组
    for (const auto &pair: DataBase) {
        Json::Value item;
        item["TargetClassName"] = get<0>(pair.second);
        item["TargetWindowName"] = get<1>(pair.second);
        item["AnimationSpeed"] = get<2>(pair.second);
        root[std::to_string(pair.first)] = item;
    }
    // 将json对象序列化为JSON格式的字符串
    std::string jsonString = root.toStyledString();
    // 将JSON格式的字符串写入文件流
    JsonData << jsonString;
    // 关闭文件流
    JsonData.close();
    return 0;
}