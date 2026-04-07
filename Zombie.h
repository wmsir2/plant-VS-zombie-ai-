// Zombie.h
#pragma once
#include <string>
#include "graphics.h" 
#include "vector2.h"  
#include <chrono>

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

    ZombieState state;
    
    int currentFrame = 0;
    int frameCount = 22;
    int frameDuration = 100;
    std::chrono::steady_clock::time_point lastUpdateTime;

    int eatingTimer = 0;
    int eatingInterval = 1000;
    Plant* targetPlant = nullptr;

public:
    Zombie(int x, int y, int hp = 100, float speed = 0.5f);
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
