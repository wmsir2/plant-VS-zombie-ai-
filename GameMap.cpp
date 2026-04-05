#include<iostream>
#include "GameMap.h"
#include "Plant.h"

GameMap::GameMap(int rows, int columns) : rows(rows), columns(columns) {
    // 初始化 map 为 rows x columns 大小的二维向量，每个元素初始为 nullptr
    map.resize(rows, std::vector<std::shared_ptr<Plant>>(columns, nullptr));
}

bool GameMap::canPlacePlant(int row, int column) const {
    // 检查指定位置是否为空（可以放置植物）
    return map[row][column] == nullptr;
}

void GameMap::placePlant(int row, int column, std::shared_ptr<Plant> plant) {
    if (canPlacePlant(row, column)) {
        map[row][column] = plant; // 放置植物
    }
}

std::shared_ptr<Plant> GameMap::getPlant(int row, int column) const {
    if (row >= 0 && row < rows && column >= 0 && column < columns) {
        return map[row][column];
    }
    return nullptr;
}

void GameMap::update() {
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < columns; ++col) {
            if (map[row][col]) {
                map[row][col]->update();
            }
        }
    }
}


void GameMap::display() {
    // 渲染地图上的植物
    int gridWidth = 80;  // 每个格子的宽度
    int gridHeight = 100; // 每个格子的高度
    int offsetX = 250;   // 网格起始位置的 X 偏移量
    int offsetY = 100;   // 网格起始位置的 Y 偏移量
    // 遍历地图上的每个位置
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < columns; ++col) {
            if (map[row][col] != nullptr) { // 如果该位置有植物
                int x = offsetX + col * gridWidth; // 计算植物的屏幕 X 坐标
                int y = offsetY + row * gridHeight;          // 计算植物的屏幕 Y 坐标
               
                map[row][col]->draw(x, y);         // 通过指针调用 draw 方法 渲染植物


            }
        }
    }
}
