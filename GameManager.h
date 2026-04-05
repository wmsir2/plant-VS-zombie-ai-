// GameManager.h
#pragma once

#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "GameMap.h"
#include "PlantCard.h"
#include "Zombie.h"
#include <vector>
#include <memory>

class GameManager {
public:
    GameManager(int rows, int columns);     // 带参构造函数
    ~GameManager();                         // 析构函数

    void startGame();           // 初始化游戏
    void displayPlantCards();   // 显示植物卡片
    void handleInput();         // 处理玩家输入
    void processClick(int x, int y);       // 处理玩家点击
    void processPlanting(int x, int y);    // 处理地图区域的种植
    void buyPlant(size_t index);           // 处理玩家购买植物
    void updateGame();          // 更新游戏状态
    void renderGame();          // 渲染游戏状态

private:
    bool running;               // 控制游戏循环
    int mouseX = 0;            // 当前鼠标的 X 坐标
    int mouseY = 0;            // 当前鼠标的 Y 坐标
    bool plantSelected = false; // 是否选择了植物卡片
    IMAGE selectedPlantImage;  // 当前选择的植物图像

    std::unique_ptr<GameMap> gameMap;  // 游戏地图
    std::vector<PlantCard> plantCards; // 植物卡片
    PlantCard* selectedCard = nullptr; // 当前选择的植物卡片

    // --- 僵尸相关 ---
    std::vector<Zombie*> zombies;      // 僵尸列表
    int zombieSpawnTimer = 0;          // 僵尸生成计时器
    int zombieSpawnInterval = 600;     // 每600帧生成一个僵尸

    void loadPlantCards();     // 加载植物卡片
    void spawnZombie();        // 生成僵尸
    void updateZombies();      // 更新僵尸
    void renderZombies();      // 渲染僵尸
    void checkZombieCollisions(); // 检测僵尸与植物碰撞
    void cleanupDeadZombies();  // 清理死亡的僵尸
};

#endif // GAMEMANAGER_H