// GameManager.cpp
#include <iostream>
#include<graphics.h>
#include <vector>
#include <algorithm>
#include <chrono>
#include <thread>
#include "GameManager.h"
#include "Sunflower.h"
#include "Peashooter.h"
#include"config.h"
#include "PlantCard.h"
#include "tool.h"

GameManager::GameManager(int rows, int columns) : running(false) {
    gameMap = std::make_unique<GameMap>(rows, columns);
}

GameManager::~GameManager() {
    // 清理僵尸
    for (auto zombie : zombies) {
        delete zombie;
    }
    zombies.clear();
}

void GameManager::startGame() {
    // 创建游戏窗口
    initgraph(WIN_WIDTH, WIN_HEIGHT, 1);
    loadPlantCards();           // 加载植物卡片
    loadimage(&imgBg, "res/白天.jpg");
    loadimage(&imgBar, "res/bar5.png");
    loadImages();               // 加载所有动画图片（包括僵尸）

    running = true;

    // 设置目标帧率为 60 FPS
    const int TARGET_FPS = 60;
    const int FRAME_TIME_MS = 1000 / TARGET_FPS;  // 约 16.67 ms
    auto frameStartTime = std::chrono::steady_clock::now();

    // 游戏主循环
    while (running) {
        auto frameBegin = std::chrono::steady_clock::now();

        handleInput();
        updateGame();
        renderGame();

        // 帧率控制
        auto frameEnd = std::chrono::steady_clock::now();
        auto frameDuration = std::chrono::duration_cast<std::chrono::milliseconds>(
            frameEnd - frameBegin).count();

        if (frameDuration < FRAME_TIME_MS) {
            std::this_thread::sleep_for(
                std::chrono::milliseconds(FRAME_TIME_MS - frameDuration));
        }
    }
}

void GameManager::handleInput() {
    // 每帧都更新鼠标位置，确保植物预览跟随光标
    MOUSEMSG msg = GetMouseMsg();
    mouseX = msg.x;
    mouseY = msg.y;

    // 处理鼠标点击事件
    if (MouseHit()) {
        msg = GetMouseMsg();
        if (msg.uMsg == WM_LBUTTONDOWN) {
            if (plantSelected) {
                processPlanting(msg.x, msg.y);
            }
            else {
                processClick(msg.x, msg.y);
            }
        }
    }
}

void GameManager::updateGame() {
    // 更新游戏地图中的植物
    gameMap->update();

    // 定时生成僵尸
    zombieSpawnTimer++;
    if (zombieSpawnTimer >= zombieSpawnInterval) {
        spawnZombie();
        zombieSpawnTimer = 0;
    }

    // 更新所有僵尸
    updateZombies();

    // 检测僵尸与植物碰撞
    checkZombieCollisions();

    // 清理死亡的僵尸
    cleanupDeadZombies();
}

void GameManager::renderGame() {
    BeginBatchDraw();

    putimage(0, 0, &imgBg);
    putimage(250, 0, &imgBar);

    displayPlantCards();

    gameMap->display();        // 渲染地图上的植物

    // 绘制所有僵尸
    renderZombies();

    // 如果选择了植物，在鼠标位置显示植物预览
    if (plantSelected) {
        putimagePNG(mouseX - 32, mouseY - 32, &selectedPlantImage);
    }

    EndBatchDraw();
}

void GameManager::processClick(int x, int y) {
    bool cardClicked = false;
    int cardY = 5;
    int cardWidth = 65;
    int cardHeight = 90;

    for (size_t i = 0; i < plantCards.size(); ++i) {
        int cardX = 338 + (int)i * cardWidth;  // 添加显式转换
        if (x >= cardX && x <= cardX + cardWidth && y >= cardY && y <= cardY + cardHeight) {
            std::cout << "点击植物卡片: " << plantCards[i].getName() << std::endl;
            cardClicked = true;
            selectedCard = &plantCards[i];
            buyPlant(i);

            std::string plantImagePath = selectedCard->getPlantImagePath();
            loadimage(&selectedPlantImage, plantImagePath.c_str());

            if (selectedPlantImage.getwidth() == 0 || selectedPlantImage.getheight() == 0) {
                std::cout << "加载植物图像失败: " << plantImagePath << std::endl;
                plantSelected = false;
            }
            else {
                plantSelected = true;
            }
            break;
        }
    }

    if (!cardClicked) {
        selectedCard = nullptr;
        plantSelected = false;
    }
}

void GameManager::processPlanting(int x, int y) {
    int gridWidth = 80;
    int gridHeight = 100;
    int row = (y - 100) / gridHeight;
    int column = (x - 250) / gridWidth;

    if (row >= 0 && row < gameMap->getRows() && column >= 0 && column < gameMap->getColumns()) {
        if (gameMap->canPlacePlant(row, column)) {
            std::cout << "在 (" << row << ", " << column << ") 种植 " << selectedCard->getName() << std::endl;
            
            if (selectedCard->getName() == "向日葵") {
                gameMap->placePlant(row, column, std::make_shared<Sunflower>(row, column));
            }
            else if (selectedCard->getName() == "豌豆射手") {
                gameMap->placePlant(row, column, std::make_shared<Peashooter>(row, column));
            }

            plantSelected = false;
            selectedCard = nullptr;
        }
        else {
            std::cout << "该位置已有植物！" << std::endl;
        }
    }
}

void GameManager::buyPlant(size_t index) {
    std::cout << "购买植物: " << plantCards[index].getName() << std::endl;
}

void GameManager::loadPlantCards() {
    plantCards.emplace_back("向日葵", "res/cards/sunflower_card.png", "res/cards/sunflower_plant.gif", 50);
    plantCards.emplace_back("豌豆射手", "res/cards/peashooter_card.png", "res/cards/peashooter_plant.gif", 100);
}

void GameManager::displayPlantCards() {
    int y = 5;

    for (size_t i = 0; i < plantCards.size(); ++i) {
        int xPos = 338 + (int)i * 65;  // 添加显式转换
        putimagePNG(xPos, y, &plantCards[i].getImage());
    }
}

// --- 僵尸相关方法 ---

void GameManager::spawnZombie() {
    // 限制场景内僵尸数量，防止性能下降
    const int MAX_ZOMBIES = 20;
    if (zombies.size() >= MAX_ZOMBIES) {
        return;  // 僵尸数量达到上限，停止生成
    }

    // 在右侧随机一行生成僵尸
    int row = rand() % gameMap->getRows();
    int spawnX = WIN_WIDTH;  // 屏幕右侧
    int spawnY = 100 + row * 100;  // 根据行数计算Y坐标

    // 速度设置：0.36f 像素/帧，在 60 FPS 下约 21.6 px/s
    // 从屏幕右侧 (900) 到左侧 (250) 需约 30 秒
    Zombie* newZombie = new Zombie(spawnX, spawnY, 100, 0.36f);
    zombies.push_back(newZombie);

    // 仅在达到特定数量时输出日志，避免过度输出
    if (zombies.size() % 5 == 0) {
        std::cout << "[Zombie] 僵尸数量: " << zombies.size() << std::endl;
    }
}

void GameManager::updateZombies() {
    for (auto zombie : zombies) {
        if (zombie->isDead()) continue;

        zombie->update();

        // 限制僵尸位置不能超过屏幕左边，防止飘出屏幕
        if (zombie->getPosition().x < 0) {
            zombie->clampPosition(0);
            std::cout << "[GAME OVER] 僵尸冲到左边了!" << std::endl;
            running = false;
            break;
        }
    }
}

void GameManager::renderZombies() {
    for (auto zombie : zombies) {
        if (!zombie->isDead()) {
            zombie->draw();
        }
    }
}

void GameManager::checkZombieCollisions() {
    for (auto zombie : zombies) {
        if (zombie->isDead() || zombie->getTargetPlant() != nullptr) {
            continue;  // 跳过已死亡或已在啃食的僵尸
        }

        // 遍历地图中的所有植物，找到第一个碰撞后立即停止
        bool foundCollision = false;
        for (int row = 0; row < gameMap->getRows() && !foundCollision; ++row) {
            for (int col = 0; col < gameMap->getColumns() && !foundCollision; ++col) {
                auto plant = gameMap->getPlant(row, col);

                if (plant != nullptr && zombie->isCollidingWith(plant.get())) {
                    zombie->startEating(plant.get());
                    foundCollision = true;  // 找到碰撞后立即退出
                }
            }
        }
    }
}

void GameManager::cleanupDeadZombies() {
    // 使用 erase-remove 惯用法提高效率
    auto oldSize = zombies.size();

    zombies.erase(
        std::remove_if(zombies.begin(), zombies.end(),
            [](Zombie* zombie) {
                if (zombie->isDead()) {
                    delete zombie;
                    return true;  // 标记为移除
                }
                return false;
            }),
        zombies.end()
    );

    // 仅在确实清理了僵尸时输出日志
    if (oldSize > zombies.size()) {
        std::cout << "[Cleanup] 僵尸已清理, 剩余数量: " << zombies.size() << std::endl;
    }
}

