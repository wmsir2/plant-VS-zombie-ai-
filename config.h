#pragma once
#include<graphics.h>
#include <string>
#define WIN_WIDTH	900	//定义游戏窗口宽度
#define WIN_HEIGHT	600	//定义游戏窗口高度
#define MAP_ROW  5		//定义草坪可种植的行数
#define MAP_COL	 9		//定义草坪可种植的列数


extern IMAGE imgBg;		//背景图片
extern IMAGE imgBar;	//植物卡片栏图片
extern IMAGE imgSunshineBall[29];	//阳光的29帧图片
extern IMAGE imgZM[22];			//僵尸行走图片数组
extern IMAGE imgZMDead[20];		//僵尸死亡的图片数组
extern IMAGE imgZMEat[21];		//僵尸吃植物的图片数组
extern IMAGE imgZmStand[11];	//僵尸站立的图片数组
extern IMAGE imgBulletNormal;		//正常情况下的子弹图片
extern IMAGE imgBullBlast[4];		//爆炸情况下的子弹图片数组

// 定义植物摇摆动画的帧数
const int SUNFLOWER_FRAME_COUNT = 18; // 向日葵的帧数
const int PEASHOOTER_FRAME_COUNT = 13; // 豌豆射手的帧数

extern IMAGE imgSunflowerFrames[SUNFLOWER_FRAME_COUNT]; // 向日葵动画帧
extern IMAGE imgPeashooterFrames[PEASHOOTER_FRAME_COUNT]; // 豌豆射手动画帧


bool fileExists(const std::string& filename);		//判断文件是否存在
void loadImages();									//加载植物摇摆动画图片
