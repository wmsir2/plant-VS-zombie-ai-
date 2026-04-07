#pragma once
#include "graphics.h"
#include <chrono>
#include "vector2.h"

class Sun {
private:
    int x, y;
    int startX, startY;
    int targetX, targetY;
    int groundY;
    bool falling = true;
    bool collected = false;
    bool flying = false;
    bool produced = false;
    vector2 p2, p3;
    float t = 0;
    float speed;
    int frameIndex = 0;
    std::chrono::steady_clock::time_point spawnTime;
    
public:
    Sun(int startX, int startY);
    static Sun* createFromFlower(int x, int y);
    void update();
    void draw();
    bool isCollected() const { return collected; }
    void collect();
    bool isClicked(int mx, int my) const;
    int getValue() const;
    bool shouldRemove() const { return !collected && t >= 1.0f; }
    bool isProduced() const { return produced; }
};
