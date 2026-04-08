#pragma once
#include "config.h"

#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "GameMap.h"
#include "PlantCard.h"
#include "Zombie.h"
#include "Bullet.h"
#include "Sun.h"
#include <vector>
#include <memory>

class GameManager {
public:
    GameManager(int rows, int columns);
    ~GameManager();

    void startGame();
    void displayPlantCards();
    void handleInput();
    void processClick(int x, int y);
    void processPlanting(int x, int y);
    void buyPlant(size_t index);
    void updateGame();
    void renderGame();

private:
    bool running;
    int mouseX = 0;
    int mouseY = 0;
    bool plantSelected = false;
    IMAGE selectedPlantImage;

    // 经济系统
    int sunshine = 50;
    int sunshineTimer = 0;

    std::unique_ptr<GameMap> gameMap;
    std::vector<PlantCard> plantCards;
    PlantCard* selectedCard = nullptr;

    // 僵尸
    std::vector<Zombie*> zombies;
    int zombieSpawnTimer = 0;
    int zombieSpawnInterval = ZOMBIE_SPAWN_INTERVAL;

    // 子弹
    std::vector<Bullet*> bullets;
    int shootTimer = 0;

    // 阳光
    std::vector<Sun*> suns;
    int sunSpawnTimer = 0;
    int sunSpawnInterval = SKY_SUN_INTERVAL;

    void loadPlantCards();
    void spawnZombie();
    void updateZombies();
    void renderZombies();
    void checkZombieCollisions();
    void cleanupDeadZombies();
    void updateBullets();
    void renderBullets();
    void checkBulletCollisions();
    void spawnSun();
    void updateSuns();
    void renderSuns();
    void checkSunClick(int x, int y);
    void renderSunshineUI();
    void tryShoot();
};

#endif
