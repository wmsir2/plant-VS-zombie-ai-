#include"config.h"
#include <fstream>
#include<iostream>
IMAGE imgBg;
IMAGE imgBar;
IMAGE imgSunshineBall[29];	//表示阳光的29帧图片
IMAGE imgZM[22];		//僵尸图片数组
IMAGE imgZMDead[20];		//僵尸死亡的图片数组
IMAGE imgZMEat[21];		//僵尸吃植物的图片数组
IMAGE imgZmStand[11];	//僵尸站立的图片数组
IMAGE imgBulletNormal;		//正常情况下的子弹图片
IMAGE imgBullBlast[4];		//爆炸情况下的子弹图片数组

IMAGE imgSunflowerFrames[SUNFLOWER_FRAME_COUNT];        //向日葵摇摆动画序列帧
IMAGE imgPeashooterFrames[PEASHOOTER_FRAME_COUNT];      //豌豆射手摇摆动画序列帧


bool fileExists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
}

void loadImages() {
    // 加载向日葵的动画帧
    for (int i = 0; i < SUNFLOWER_FRAME_COUNT; ++i) {
        std::string filename = "res/Plant/sunflower/" + std::to_string(i+1) + ".png"; // 输出路径
        if (fileExists(filename)) {
            loadimage(&imgSunflowerFrames[i], filename.c_str());
        }
        else {
            std::cerr << "Failed to load image: " << filename << std::endl;
        }
    }

    // 加载豌豆射手的动画帧
    for (int i = 0; i < PEASHOOTER_FRAME_COUNT; ++i) {
        std::string filename = "res/Plant/peashooter/" + std::to_string(i+1) + ".png";
        if (fileExists(filename)) {
            loadimage(&imgPeashooterFrames[i], filename.c_str());
        }
        else {
            std::cerr << "Failed to load image: " << filename << std::endl;
        }
    }
    // 加载僵尸行走动画帧（22帧）
    std::cout << "正在加载僵尸行走动画..." << std::endl;
    for (int i = 0; i < 22; ++i) {
        std::string filename = "res/zm/" + std::to_string(i + 1) + ".png";
        if (fileExists(filename)) {
            loadimage(&imgZM[i], filename.c_str());
        }
        else {
            std::cerr << "Failed to load zombie walk image: " << filename << std::endl;
        }
    }
    std::cout << "僵尸行走动画加载完毕！" << std::endl;

    // 加载僵尸啃食动画帧（21帧）
    std::cout << "正在加载僵尸啃食动画..." << std::endl;
    for (int i = 0; i < 21; ++i) {
        std::string filename = "res/zm_eat/" + std::to_string(i + 1) + ".png";
        if (fileExists(filename)) {
            loadimage(&imgZMEat[i], filename.c_str());
        }
        else {
            std::cerr << "Failed to load zombie eat image: " << filename << std::endl;
        }
    }
    std::cout << "僵尸啃食动画加载完毕！" << std::endl;
}
