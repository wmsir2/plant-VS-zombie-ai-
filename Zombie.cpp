#include "Zombie.h"
#include "Plant.h"
#include "tool.h"
#include "config.h"
#include <iostream>
#include <chrono>

Zombie::Zombie(int x, int y, int hp, float speed, int row)
    : blood(hp), moveSpeed(speed), position(x, y), spawnRow(row),
    attackPower(10), state(ZombieState::WALKING),
    currentFrame(0), frameDuration(100) {
    lastUpdateTime = std::chrono::steady_clock::now();
    eatingTimer = 0;
    targetPlant = nullptr;
}

Zombie::~Zombie() {}

void Zombie::update() {
    switch (state) {
    case ZombieState::WALKING:
        position.x -= moveSpeed;
        break;
    case ZombieState::EATING:
        eatingTimer += frameDuration;
        if (eatingTimer >= eatingInterval && targetPlant != nullptr) {
            targetPlant->takeDamage(attackPower);
            if (targetPlant->getBlood() <= 0) {
                stopEating();
            }
            eatingTimer = 0;
        }
        break;
    case ZombieState::DEAD:
        return;
    }
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastUpdateTime).count();
    if (elapsed >= frameDuration) {
        if (state == ZombieState::WALKING) currentFrame = (currentFrame + 1) % 22;
        else if (state == ZombieState::EATING) currentFrame = (currentFrame + 1) % 21;
        lastUpdateTime = now;
    }
}

void Zombie::draw() {
    if (isDead()) return;
    IMAGE* currentImg = nullptr;
    if (state == ZombieState::WALKING) currentImg = &imgZM[currentFrame];
    else if (state == ZombieState::EATING) currentImg = &imgZMEat[currentFrame];
    if (currentImg) {
        int zombieHeight = currentImg->getheight();
        int gridHeight = 100;
        // 向上调整，让僵尸顶部更靠近格子顶部
        int drawY = position.y - (zombieHeight - gridHeight) / 2 - 50;
        putimagePNG(position.x, drawY, currentImg);
    }
}

void Zombie::takeDamage(int amount) {
    blood -= amount;
    if (blood <= 0) {
        state = ZombieState::DEAD;
    }
}

void Zombie::startEating(Plant* plant) {
    if (plant != nullptr) {
        state = ZombieState::EATING;
        targetPlant = plant;
        currentFrame = 0;
        eatingTimer = 0;
    }
}

void Zombie::stopEating() {
    state = ZombieState::WALKING;
    targetPlant = nullptr;
    currentFrame = 0;
    eatingTimer = 0;
}

int Zombie::getRow() const {
    return spawnRow;
}

bool Zombie::isCollidingWith(Plant* plant) const {
    if (plant == nullptr) return false;
    if (getRow() != plant->getRow()) return false;

    int gridWidth = GRID_WIDTH;
    int offsetX = MAP_OFFSET_X;
    
    // 植物格子位置
    int plantX = offsetX + plant->getCol() * gridWidth;
    int x1 = plantX + 10;  // 植物左边界
    int x2 = plantX + 60;  // 植物右边界
    int x3 = position.x + 80;  // 僵尸前缘(右手边)

    // 判断僵尸是否与植物发生碰撞
    if (x3 > x1 && x3 < x2) {
        return true;
    }
    return false;
}

void Zombie::loadResources() {}

void Zombie::clampPosition(int minX) {
    if (position.x < minX) position.x = minX;
}

