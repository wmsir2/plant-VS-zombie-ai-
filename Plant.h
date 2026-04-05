// Plant.h
#pragma once
#include <string>
#include <graphics.h>
#include <chrono>
class Plant {
protected:
    std::string name;
    int blood;
    int row, col;
    IMAGE img; // 用于存储植物的图像
    int currentFrame = 0;
    std::chrono::steady_clock::time_point lastUpdateTime;
    int frameDuration = 80; // 每帧持续时间，单位毫秒
public:
    Plant();

    Plant(const std::string& name, int blood, int row=0, int col=0);

    virtual ~Plant();
    virtual void draw(int x, int y)  = 0; // 抽象方法，绘制植物
    virtual void update() = 0;           // 纯虚函数

    int getBlood() const;
    void takeDamage(int amount);
    std::string getName() const;
    int getRow() const;
    int getCol() const;
    void setPosition(int row, int col);
};
