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
    for (auto zombie : zombies) {
        delete zombie;
    }
    zombies.clear();
}

void GameManager::startGame() {
    initgraph(WIN_WIDTH, WIN_HEIGHT, 1);
    loadPlantCards();
    loadimage(&imgBg, "res/白天.jpg");
    loadimage(&imgBar, "res/bar5.png");
    loadImages();

    running = true;

    const int TARGET_FPS = 60;
    const int FRAME_TIME_MS = 1000 / TARGET_FPS;

    while (running) {
        auto frameBegin = std::chrono::steady_clock::now();

        handleInput();
        updateGame();
        renderGame();

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
    while (MouseHit()) {
        MOUSEMSG msg = GetMouseMsg();
        mouseX = msg.x;
        mouseY = msg.y;

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
    gameMap->update();

    zombieSpawnTimer++;
    if (zombieSpawnTimer >= zombieSpawnInterval) {
        spawnZombie();
        zombieSpawnTimer = 0;
    }

    updateZombies();
    checkZombieCollisions();
    gameMap->removeDeadPlants();
    cleanupDeadZombies();
}

void GameManager::renderGame() {
    BeginBatchDraw();

    putimage(0, 0, &imgBg);
    putimage(250, 0, &imgBar);

    displayPlantCards();
    gameMap->display();
    renderZombies();

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
        int cardX = 338 + (int)i * cardWidth;
        if (x >= cardX && x <= cardX + cardWidth && y >= cardY && y <= cardY + cardHeight) {
            std::cout << "Click card: " << plantCards[i].getName() << std::endl;
            cardClicked = true;
            selectedCard = &plantCards[i];
            buyPlant(i);

            std::string plantImagePath = selectedCard->getPlantImagePath();
            loadimage(&selectedPlantImage, plantImagePath.c_str());

            if (selectedPlantImage.getwidth() == 0 || selectedPlantImage.getheight() == 0) {
                std::cout << "Load failed: " << plantImagePath << std::endl;
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
            std::cout << "Plant at (" << row << ", " << column << ") " << selectedCard->getName() << std::endl;
            
            if (selectedCard->getName() == "向日葵") {
                gameMap->placePlant(row, column, std::make_shared<Sunflower>(row, column));
            }
            else if (selectedCard->getName() == "豌豆射手") {
                gameMap->placePlant(row, column, std::make_shared<Peashooter>(row, column));
            }

            plantSelected = false;
            selectedCard = nullptr;
        }
    }
}

void GameManager::buyPlant(size_t index) {
    std::cout << "Buy: " << plantCards[index].getName() << std::endl;
}

void GameManager::loadPlantCards() {
    plantCards.emplace_back("向日葵", "res/cards/sunflower_card.png", "res/cards/sunflower_plant.gif", 50);
    plantCards.emplace_back("豌豆射手", "res/cards/peashooter_card.png", "res/cards/peashooter_plant.gif", 100);
}

void GameManager::displayPlantCards() {
    int y = 5;
    for (size_t i = 0; i < plantCards.size(); ++i) {
        int xPos = 338 + (int)i * 65;
        putimagePNG(xPos, y, &plantCards[i].getImage());
    }
}

void GameManager::spawnZombie() {
    const int MAX_ZOMBIES = 20;
    if (zombies.size() >= MAX_ZOMBIES) return;

    int row = rand() % gameMap->getRows();
    int spawnX = WIN_WIDTH;
    int spawnY = 100 + row * 100;

    Zombie* newZombie = new Zombie(spawnX, spawnY, 100, 0.36f);
    zombies.push_back(newZombie);

    std::cout << "Zombie spawned at row " << row << std::endl;
}

void GameManager::updateZombies() {
    for (auto zombie : zombies) {
        if (zombie->isDead()) continue;
        zombie->update();
        if (zombie->getPosition().x < 0) {
            zombie->clampPosition(0);
            std::cout << "[GAME OVER]" << std::endl;
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
        if (zombie->isDead() || zombie->getTargetPlant() != nullptr) continue;

        bool foundCollision = false;
        for (int row = 0; row < gameMap->getRows() && !foundCollision; ++row) {
            for (int col = 0; col < gameMap->getColumns() && !foundCollision; ++col) {
                auto plant = gameMap->getPlant(row, col);
                if (plant != nullptr && zombie->isCollidingWith(plant.get())) {
                    zombie->startEating(plant.get());
                    foundCollision = true;
                }
            }
        }
    }
}

void GameManager::cleanupDeadZombies() {
    zombies.erase(
        std::remove_if(zombies.begin(), zombies.end(),
            [](Zombie* zombie) {
                if (zombie->isDead()) {
                    delete zombie;
                    return true;
                }
                return false;
            }),
        zombies.end());
}
