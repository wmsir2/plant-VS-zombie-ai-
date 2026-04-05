// Zombie.h
#pragma once
#include <string>
#include "graphics.h" 
#include "vector2.h"  
#include <chrono>

// 前向声明（因为我们还没有写 Plant，这里先声明，避免循环包含）
class Plant;

enum class ZombieState {
    WALKING,
    EATING,
    DEAD
};

class Zombie {
private:
    int blood;          // 血量
    int attackPower;    // 啃食伤害
    vector2 position;   // 位置
    float moveSpeed;    // 移动速度（每帧移动的像素）

    ZombieState state;  // 当前状态
    
    // 动画相关
    int currentFrame = 0;
    int frameCount = 22;                // 行走帧数
    int frameDuration = 100;            // 每帧持续时间（毫秒）
    std::chrono::steady_clock::time_point lastUpdateTime;

    // 啃食相关
    int eatingTimer = 0;                // 啃食计时器（毫秒）
    int eatingInterval = 1000;          // 每次啃食间隔（毫秒）
    Plant* targetPlant = nullptr;       // 当前目标植物

public:
    Zombie(int x, int y, int hp = 100, float speed = 0.5f);
    ~Zombie();

    void update();                      // 更新状态（移动/死亡判断）
    void draw();                        // 绘制自己
    void takeDamage(int amount);        // 受击
    void startEating(Plant* plant);     // 开始啃食
    void stopEating();                  // 停止啃食
    void clampPosition(int minX);       // 限制位置不低于 minX
    bool isDead() const { return state == ZombieState::DEAD; }
    bool isCollidingWith(Plant* plant) const;  // 碰撞检测（添加 const）
    vector2 getPosition() const { return position; }
    int getBlood() const { return blood; }
    Plant* getTargetPlant() const { return targetPlant; }

    // 静态方法：加载资源
    static void loadResources();
};
