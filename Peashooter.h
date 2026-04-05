#pragma once
#include "Plant.h"
#include "config.h"

class Peashooter : public Plant {
private:
    int currentFrame;
public:
    Peashooter();                  // 默认构造
    Peashooter(int row, int col);  // 指定位置构造
    virtual void update() override;
    virtual void draw(int x, int y) override;
};
