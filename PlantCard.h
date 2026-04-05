// PlantCard.h
#pragma once
#include <string>
#include <graphics.h>
#include<iostream>

class PlantCard {
public:

    PlantCard(const std::string& name, const std::string& cardImagePath, const std::string& plantImagePath, int cost);
    void display(int x, int y); // 在指定位置显示卡片

    const std::string& getPlantImagePath() const; // 新增：获取植物图像路径
    std::string getName() const; // 获取植物名称
    int getCost() const; // 获取植物价格
    IMAGE& getImage();// 获取图像
private:
    std::string name; // 植物名称
    IMAGE image; // 植物卡片图像
    int cost; // 植物价格
    std::string imagePath; // 卡片图片路径
    std::string plantImagePath; // 植物图像路径
};
