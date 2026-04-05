#include<iostream>
#include<graphics.h>
#include"tool.h"
#include"config.h"
#include "GameManager.h"
using namespace std;


int main() {
    // 创建游戏管理器，设地图大小为 MAP_ROW * MAP_COL
    GameManager gameManager(MAP_ROW, MAP_COL);

    // 启动游戏
    gameManager.startGame();

    return 0;
}

