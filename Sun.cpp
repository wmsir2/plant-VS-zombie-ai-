#include "Sun.h"
#include "config.h"
#include "tool.h"
#include <cmath>
#include <cstdlib>

Sun::Sun(int startX, int startY) : startX(startX), startY(startY), 
    x(startX), y(startY - 100), targetX(SUN_SLOT_X), targetY(SUN_SLOT_Y) {
    spawnTime = std::chrono::steady_clock::now();
    groundY = SUN_GROUND_Y_BASE + (rand() % MAP_ROW) * SUN_GROUND_Y_INTERVAL;
    float distance = groundY - (startY - 100);
    speed = 1.0f / (distance / SUN_FALL_SPEED);
}

Sun* Sun::createFromFlower(int x, int y) {
    Sun* sun = new Sun(x, y);
    sun->falling = false;
    sun->produced = true;
    sun->x = x;
    sun->y = y;
    sun->startX = x;
    sun->startY = y;
    int w = (100 + rand() % 50) * (rand() % 2 ? 1 : -1);
    sun->targetX = x + w;
    sun->targetY = y + 50;
    sun->p2 = vector2(x + w * 0.3, y - FLOWER_SUN_BEZIER_HEIGHT);
    sun->p3 = vector2(x + w * 0.7, y - FLOWER_SUN_BEZIER_HEIGHT);
    sun->speed = FLOWER_SUN_BEZIER_SPEED;
    sun->t = 0;
    return sun;
}

void Sun::update() {
    frameIndex = (frameIndex + 1) % 29;
    if (produced && !collected) {
        t += speed;
        x = calcBezierPoint(t, vector2(startX, startY), p2, p3, vector2(targetX, targetY)).x;
        y = calcBezierPoint(t, vector2(startX, startY), p2, p3, vector2(targetX, targetY)).y;
        if (t >= 1.0f) {
            produced = false;
            t = 0;
            falling = false;
            groundY = targetY;
            startY = groundY;
            targetX = SUN_SLOT_X;
            targetY = SUN_SLOT_Y;
        }
        return;
    }
    if (falling) {
        t += speed;
        y = (startY - 100) + t * (groundY - (startY - 100));
        if (t >= 1.0f) {
            y = groundY;
            t = 0;
            falling = false;
            startY = groundY;
        }
    }
    else if (flying) {
        t += speed;
        x = startX + t * (targetX - startX);
        y = startY + t * (targetY - startY);
        if (t >= 1.0f) {
            t = 1.0f;
            collected = true;
        }
    }
}

void Sun::draw() {
    if (flying && t >= 1.0f) return;
    putimagePNG(x, y, &imgSunshineBall[frameIndex]);
}

void Sun::collect() {
    if (flying) return;
    falling = false;
    produced = false;
    flying = true;
    startX = x;
    startY = y;
    float distance = std::sqrt((float)(targetX - startX) * (targetX - startX) + 
                               (float)(targetY - startY) * (targetY - startY));
    speed = SUN_COLLECT_SPEED / distance;
    t = 0;
}

bool Sun::isClicked(int mx, int my) const {
    if (flying) return false;
    int dx = mx - (x + 20);
    int dy = my - (y + 20);
    return std::sqrt(dx*dx + dy*dy) < 30;
}

int Sun::getValue() const {
    return SUN_VALUE;
}
