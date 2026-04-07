#include"config.h"
#include <fstream>
#include<iostream>
IMAGE imgBg;
IMAGE imgBar;
IMAGE imgSunshineBall[29];
IMAGE imgZM[22];
IMAGE imgZMDead[20];
IMAGE imgZMEat[21];
IMAGE imgZmStand[11];
IMAGE imgBulletNormal;
IMAGE imgBullBlast[4];

IMAGE imgSunflowerFrames[SUNFLOWER_FRAME_COUNT];
IMAGE imgPeashooterFrames[PEASHOOTER_FRAME_COUNT];

bool fileExists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
}

void loadImages() {
    // 加载向日葵的动画帧
    for (int i = 0; i < SUNFLOWER_FRAME_COUNT; ++i) {
        std::string filename = "res/Plant/sunflower/" + std::to_string(i+1) + ".png";
        if (fileExists(filename)) loadimage(&imgSunflowerFrames[i], filename.c_str());
    }

    // 加载豌豆射手的动画帧
    for (int i = 0; i < PEASHOOTER_FRAME_COUNT; ++i) {
        std::string filename = "res/Plant/peashooter/" + std::to_string(i+1) + ".png";
        if (fileExists(filename)) loadimage(&imgPeashooterFrames[i], filename.c_str());
    }

    // 加载僵尸行走动画帧
    for (int i = 0; i < 22; ++i) {
        std::string filename = "res/zm/" + std::to_string(i + 1) + ".png";
        if (fileExists(filename)) loadimage(&imgZM[i], filename.c_str());
    }

    // 加载僵尸啃食动画帧
    for (int i = 0; i < 21; ++i) {
        std::string filename = "res/zm_eat/" + std::to_string(i + 1) + ".png";
        if (fileExists(filename)) loadimage(&imgZMEat[i], filename.c_str());
    }

    // 加载子弹图片
    loadimage(&imgBulletNormal, "res/bullets/bullet_normal.png");
    
    // 加载阳光图片
    for (int i = 0; i < 29; ++i) {
        std::string filename = "res/sunshine/" + std::to_string(i + 1) + ".png";
        if (fileExists(filename)) loadimage(&imgSunshineBall[i], filename.c_str());
    }
}
