#pragma once
#include "graphics.h"
#include "config.h"

class Bullet {
private:
    int x, y;
    int row;
    int damage = 20;
    int speed = 8;
    bool active = true;

public:
    Bullet(int startX, int startY, int row);
    void update();
    void draw();
    bool isActive() const { return active; }
    void setInactive() { active = false; }
    int getX() const { return x; }
    int getY() const { return y; }
    int getRow() const { return row; }
    int getDamage() const { return damage; }
};
