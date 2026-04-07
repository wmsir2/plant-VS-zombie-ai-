#include "Sun.h"
#include "config.h"
#include "tool.h"
#include <cmath>

Sun::Sun(int startX, int startY) : startX(startX), startY(startY), 
    x(startX), y(startY - 100), groundY(startY), targetX(262), targetY(0) {
    spawnTime = std::chrono::steady_clock::now();
    // 计算下落速度
    float distance = groundY - (startY - 100);
    speed = 1.0f / (distance / 2.0f);
}

void Sun::update() {
    if (falling) {
        t += speed;
        y = (startY - 100) + t * (groundY - (startY - 100));
        if (t >= 1.0f) {
            y = groundY;
            t = 0;
            falling = false;
        }
    }
    else if (flying) {
        // 直线飞向阳光槽
        t += speed;
        x = startX + t * (targetX - startX);
        y = startY + t * (targetY - startY);
        if (t >= 1.0f) {
            // 飞行完成
            t = 1.0f;
        }
    }
}

void Sun::draw() {
    if (!collected) {
        putimagePNG(x, y, &imgSunshineBall[0]);
    }
}

void Sun::collect() {
    if (collected) return;
    
    collected = true;
    flying = true;
    startX = x;
    startY = y;
    
    // 计算飞向阳光槽的速度
    float distance = std::sqrt((float)(targetX - startX) * (targetX - startX) + 
                               (float)(targetY - startY) * (targetY - startY));
    speed = 8.0f / distance;  // 速度为8像素/帧
    t = 0;
}

bool Sun::isClicked(int mx, int my) const {
    if (flying) return false;  // 飞行中不可点击
    int dx = mx - (x + 20);
    int dy = my - (y + 20);
    return std::sqrt(dx*dx + dy*dy) < 30;
}
