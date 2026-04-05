#pragma once
#include "Plant.h"
#include "config.h"

class Sunflower : public Plant {
private:
    int currentFrame;
public:
    Sunflower();                  // 默认构造
    Sunflower(int row, int col);  // 指定位置构造
    virtual void update() override;
    virtual void draw(int x, int y) override;
};
