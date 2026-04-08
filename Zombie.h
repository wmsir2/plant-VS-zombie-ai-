// Zombie.h
#pragma once
#include <string>
#include "graphics.h" 
#include "vector2.h"  
#include <chrono>
#include "config.h"

class Plant;

enum class ZombieState {
    WALKING,
    EATING,
    DEAD
};

class Zombie {
private:
    int blood;
    int attackPower;
    vector2 position;
    float moveSpeed;
    int spawnRow;
    float moveAccumulator = 0.0f;

    ZombieState state;
    
    int currentFrame = 0;
    int frameDuration = ZOMBIE_FRAME_DURATION;
    std::chrono::steady_clock::time_point lastUpdateTime;

    int eatingTimer = 0;
    int eatingInterval = ZOMBIE_EAT_INTERVAL;
    Plant* targetPlant = nullptr;

public:
    Zombie(int x, int y, int hp = 100, float speed=ZOMBIE_MOVE_SPEED, int row=0);
    ~Zombie();

    void update();
    void draw();
    void takeDamage(int amount);
    void startEating(Plant* plant);
    void stopEating();
    void clampPosition(int minX);
    bool isDead() const { return state == ZombieState::DEAD; }
    bool isCollidingWith(Plant* plant) const;
    vector2 getPosition() const { return position; }
    int getBlood() const { return blood; }
    Plant* getTargetPlant() const { return targetPlant; }
    int getRow() const;

    static void loadResources();
};
