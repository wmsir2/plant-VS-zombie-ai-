#pragma once
#include "graphics.h"
#include <chrono>

class Sun {
private:
    int x, y;           // 当前坐标
    int startX, startY; // 起点（点击时的位置）
    int targetX, targetY; // 终点（阳光槽位置）
    int groundY;        // 地面位置（下落目标位置）
    bool falling = true;   // 是否正在下落
    bool collected = false; // 是否已被收集
    bool flying = false;   // 是否正在飞向阳光槽
    float t = 0;           // 插值进度 [0,1]
    float speed;           // 飞行速度
    std::chrono::steady_clock::time_point spawnTime;
    
public:
    Sun(int startX, int startY);
    void update();
    void draw();
    bool isCollected() const { return collected; }
    void collect();
    bool isClicked(int mx, int my) const;
    int getValue() const { return 25; }
    bool shouldRemove() const { return !collected && t >= 1.0f; }
};
