// Plant.cpp
#include "Plant.h"
#include <chrono>

Plant::Plant() {}

Plant::Plant(const std::string& name, int blood, int row, int col)
    : name(name), blood(blood), row(row), col(col) {
    lastUpdateTime = std::chrono::steady_clock::now();
}

// 析构函数
Plant::~Plant() {}

// 获取植物当前血量
int Plant::getBlood() const {
    return blood;
}

// 对植物造成伤害
void Plant::takeDamage(int amount) {
    blood -= amount;
    if (blood < 0) blood = 0;
}

// 获取植物名称
std::string Plant::getName() const {
    return name;
}

// 获取植物所在行
int Plant::getRow() const {
    return row;
}

// 获取植物所在列
int Plant::getCol() const {
    return col;
}

// 设置植物位置
void Plant::setPosition(int newRow, int newCol) {
    row = newRow;
    col = newCol;
}
