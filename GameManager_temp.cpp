#include <iostream>

#include <graphics.h>

#include <vector>

#include <algorithm>

#include <chrono>

#include <thread>

#include "GameManager.h"

#include "Sunflower.h"

#include "Peashooter.h"

#include "config.h"

#include "PlantCard.h"

#include "tool.h"



GameManager::GameManager(int rows, int columns) : running(false) {

    gameMap = std::make_unique<GameMap>(rows, columns);

}



GameManager::~GameManager() {

    for (auto zombie : zombies) delete zombie;

    for (auto bullet : bullets) delete bullet;

    for (auto sun : suns) delete sun;

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

        auto frameDuration = std::chrono::duration_cast<std::chrono::milliseconds>(frameEnd - frameBegin).count();



        if (frameDuration < FRAME_TIME_MS) {

            std::this_thread::sleep_for(std::chrono::milliseconds(FRAME_TIME_MS - frameDuration));

        }

    }

}



void GameManager::handleInput() {

    while (MouseHit()) {

        MOUSEMSG msg = GetMouseMsg();

        mouseX = msg.x;

        mouseY = msg.y;



        if (msg.uMsg == WM_LBUTTONDOWN) {

            checkSunClick(msg.x, msg.y);

            if (plantSelected) {

                processPlanting(msg.x, msg.y);

            } else {

                processClick(msg.x, msg.y);

            }

        }

    }

}



void GameManager::updateGame() {

    gameMap->update();



    // 僵尸生成

    zombieSpawnTimer++;

    if (zombieSpawnTimer >= zombieSpawnInterval) {

        spawnZombie();

        zombieSpawnTimer = 0;

    }



    // 阳光生成

    sunSpawnTimer++;

    if (sunSpawnTimer >= 500) {

        spawnSun();

        sunSpawnTimer = 0;

    }



    // 向日葵产阳光

    for (int row = 0; row < gameMap->getRows(); row++) {

        for (int col = 0; col < gameMap->getColumns(); col++) {

            auto plant = gameMap->getPlant(row, col);

            if (plant && plant->getName() == "向日葵") {

                sunshineTimer++;

                if (sunshineTimer >= 600) {

                    int sunX = 250 + col * 80 + 40;

                    int sunY = 100 + row * 100;

                    suns.push_back(new Sun(sunX, sunY));

                    sunshineTimer = 0;

                }

            }

        }

    }



    updateZombies();

    checkZombieCollisions();

    gameMap->removeDeadPlants();

    cleanupDeadZombies();



    // 子弹更新

    tryShoot();

    updateBullets();

    checkBulletCollisions();



    // 阳光更新

    updateSuns();`n    }

}



void GameManager::renderGame() {

    BeginBatchDraw();



    putimage(0, 0, &imgBg);

    putimage(250, 0, &imgBar);



    displayPlantCards();

    renderSunshineUI();

    gameMap->display();

    renderZombies();

    renderBullets();

    renderSuns();



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

            if (sunshine >= plantCards[i].getCost()) {

                sunshine -= plantCards[i].getCost();

                selectedCard = &plantCards[i];

                std::string plantImagePath = selectedCard->getPlantImagePath();

                loadimage(&selectedPlantImage, plantImagePath.c_str());

                if (selectedPlantImage.getwidth() == 0 || selectedPlantImage.getheight() == 0) {

                    plantSelected = false;

                } else {

                    plantSelected = true;

                }

            }

            cardClicked = true;

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

            if (selectedCard->getName() == "向日葵") {

                gameMap->placePlant(row, column, std::make_shared<Sunflower>(row, column));

            } else if (selectedCard->getName() == "豌豆射手") {

                gameMap->placePlant(row, column, std::make_shared<Peashooter>(row, column));

            }

            plantSelected = false;

            selectedCard = nullptr;

        }

    }

}



void GameManager::buyPlant(size_t index) {

    // 已合并到processClick中

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



    zombies.push_back(new Zombie(spawnX, spawnY, 100, 0.36f));

}



void GameManager::spawnSun() {

    int col = rand() % gameMap->getColumns();

    int sunX = 250 + col * 80 + 40;

    suns.push_back(new Sun(sunX, 50));

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



        for (int row = 0; row < gameMap->getRows(); ++row) {

            for (int col = 0; col < gameMap->getColumns(); ++col) {

                auto plant = gameMap->getPlant(row, col);

                if (plant && zombie->isCollidingWith(plant.get())) {

                    zombie->startEating(plant.get());

                    break;

                }

            }

        }

    }

}



void GameManager::cleanupDeadZombies() {

    zombies.erase(std::remove_if(zombies.begin(), zombies.end(), [](Zombie* z) {

        if (z->isDead()) { delete z; return true; }

        return false;

    }), zombies.end());

}



void GameManager::tryShoot() {

    shootTimer++;

    if (shootTimer < 90) return;



    for (int row = 0; row < gameMap->getRows(); row++) {

        for (int col = 0; col < gameMap->getColumns(); col++) {

            auto plant = gameMap->getPlant(row, col);

            if (plant && plant->getName() == "豌豆射手") {

                // 检查该行是否有僵尸

                bool hasZombie = false;

                for (auto zombie : zombies) {

                    if (zombie->getRow() == row && !zombie->isDead() && zombie->getPosition().x > 250 + col * 80) {

                        hasZombie = true;

                        break;

                    }

                }

                if (hasZombie) {

                    int bulletX = 250 + col * 80 + 60;

                    int bulletY = 100 + row * 100 + 40;

                    bullets.push_back(new Bullet(bulletX, bulletY, row));

                    shootTimer = 0;

                }

            }

        }

    }

}



void GameManager::updateBullets() {

    for (auto bullet : bullets) {

        bullet->update();

    }

    bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](Bullet* b) {

        if (!b->isActive()) { delete b; return true; }

        return false;

    }), bullets.end());

}



void GameManager::renderBullets() {

    for (auto bullet : bullets) {

        bullet->draw();

    }

}



void GameManager::checkBulletCollisions() {

    for (auto bullet : bullets) {

        if (!bullet->isActive()) continue;

        for (auto zombie : zombies) {

            if (zombie->isDead()) continue;

            int bX = bullet->getX();

            int bY = bullet->getY();

            int zX = zombie->getPosition().x;

            int zY = zombie->getPosition().y;

            if (bullet->getRow() == zombie->getRow() && bX >= zX && bX <= zX + 50) {

                zombie->takeDamage(bullet->getDamage());

                bullet->setInactive();

                break;

            }

        }

    }

}



    // 移除已飞行完成的阳光并增加阳光值

        return false;

    }), suns.end());

}

        return false;

    }), suns.end());

}



void GameManager::checkSunClick(int x, int y) {

    for (auto sun : suns) {

        if (!sun->isCollected() && sun->isClicked(x, y)) {

            sun->collect();

            break;

        }

    }

}



void GameManager::updateSuns() {
    for (auto sun : suns) {
        sun->update();
    }
    // 移除已收集的阳光并增加阳光值
    suns.erase(std::remove_if(suns.begin(), suns.end(), [this](Sun* s) {
        if (s->isCollected()) {
            sunshine += s->getValue();
            delete s;
            return true;
        }
        return false;
    }), suns.end());
}

void GameManager::renderSuns() {

    for (auto sun : suns) {

        sun->draw();

    }

}







void GameManager::renderSunshineUI() {

    // 在游戏画面顶部栏显示阳光数量 (参考原版位置: 282, 67)

    settextcolor(RGB(255, 255, 255));

    setbkmode(TRANSPARENT);

    settextstyle(25, 0, _T("Arial"));

    

    TCHAR buf[32];

    _stprintf_s(buf, _T("%d"), sunshine);

    outtextxy(280, 67, buf);

}


















