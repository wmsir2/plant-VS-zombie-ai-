#include "Zombie.h"
#include "Plant.h"
#include "tool.h"
#include "config.h"
#include <iostream>
#include <chrono>

// --- 构造函数 ---
Zombie::Zombie(int x, int y, int hp, float speed)
    : blood(hp), moveSpeed(speed), position(x, y),
    attackPower(10), state(ZombieState::WALKING),
    currentFrame(0), frameDuration(100) {
    lastUpdateTime = std::chrono::steady_clock::now();
    eatingTimer = 0;
    targetPlant = nullptr;
}

// --- 析构函数 ---
Zombie::~Zombie() {
    // 清理资源
}

// --- 1. 核心更新逻辑 (状态机) ---
void Zombie::update() {
    switch (state) {
    case ZombieState::WALKING: {
        // 向左移动
        position.x -= moveSpeed;

        // 检测是否出屏幕（游戏失败条件）
        if (position.x < 0) {
            std::cout << "Zombie 冲到屏幕左边了！游戏失败！" << std::endl;
            // TODO: 触发游戏失败逻辑
        }
        break;
    }

    case ZombieState::EATING: {
        // 啃食计时器增加
        eatingTimer += frameDuration;

        // 每隔 eatingInterval 毫秒，对植物造成一次伤害
        if (eatingTimer >= eatingInterval && targetPlant != nullptr) {
            targetPlant->takeDamage(attackPower);
            std::cout << "Zombie 啃食了植物！对其造成 " << attackPower << " 点伤害" << std::endl;

            // 检查植物是否已死亡
            if (targetPlant->getBlood() <= 0) {
                std::cout << "植物被吃掉了！Zombie 继续前进" << std::endl;
                stopEating();
            }

            eatingTimer = 0;
        }
        break;
    }

    case ZombieState::DEAD:
        // 死亡状态，不做任何更新
        return;
    }

    // --- 动画更新逻辑 ---
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
        now - lastUpdateTime).count();

    if (elapsed >= frameDuration) {
        if (state == ZombieState::WALKING) {
            currentFrame = (currentFrame + 1) % 22;  // 行走动画有22帧
        }
        else if (state == ZombieState::EATING) {
            currentFrame = (currentFrame + 1) % 21;  // 啃食动画有21帧
        }
        lastUpdateTime = now;
    }
}

// --- 2. 绘制逻辑 (外观) ---
void Zombie::draw() {
    if (isDead()) return;

    IMAGE* currentImg = nullptr;

    // 根据状态选择不同的图像（使用 config.cpp 中的全局变量）
    if (state == ZombieState::WALKING) {
        currentImg = &imgZM[currentFrame];
    }
    else if (state == ZombieState::EATING) {
        currentImg = &imgZMEat[currentFrame];
    }

    if (currentImg) {
        // 调用透明贴图函数
        putimagePNG(position.x, position.y, currentImg);
    }
}

// --- 3. 交互逻辑 (反应) ---
void Zombie::takeDamage(int amount) {
    blood -= amount;
    std::cout << "Zombie 受到了 " << amount << " 点伤害！剩余血量：" << blood << std::endl;

    if (blood <= 0) {
        state = ZombieState::DEAD;
        std::cout << "Zombie 死亡！" << std::endl;
    }
}

void Zombie::startEating(Plant* plant) {
    if (plant != nullptr) {
        state = ZombieState::EATING;
        targetPlant = plant;
        currentFrame = 0;  // 重置帧数
        eatingTimer = 0;
        std::cout << "Zombie 开始啃食！" << std::endl;
    }
}

void Zombie::stopEating() {
    state = ZombieState::WALKING;
    targetPlant = nullptr;
    currentFrame = 0;  // 重置帧数
    eatingTimer = 0;
    std::cout << "Zombie 停止啃食，继续前进！" << std::endl;
}

// --- 4. 碰撞检测 (感官) ---
bool Zombie::isCollidingWith(Plant* plant) const {
    if (plant == nullptr) return false;

    // 将逻辑坐标转换为屏幕坐标（每个格子80×100像素）
    int gridWidth = 80;
    int gridHeight = 100;
    int offsetX = 250;    // 地图起始X
    int offsetY = 100;    // 地图起始Y

    int plantX = offsetX + plant->getCol() * gridWidth;
    int plantY = offsetY + plant->getRow() * gridHeight;

    // 僵尸尺寸（宽50高80）与植物尺寸（宽80高100）的碰撞检测
    int zombieWidth = 50;
    int zombieHeight = 80;
    int plantWidth = 80;
    int plantHeight = 100;

    // 矩形碰撞检测
    if (position.x + zombieWidth >= plantX &&
        position.x <= plantX + plantWidth &&
        position.y + zombieHeight >= plantY &&
        position.y <= plantY + plantHeight) {
        return true;
    }
    return false;
}

// --- 5. 限制位置 ---
void Zombie::clampPosition(int minX) {
    if (position.x < minX) {
        position.x = minX;
    }
}