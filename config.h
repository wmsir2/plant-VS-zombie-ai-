#pragma once
#include<graphics.h>
#include <string>

// ============================================
// 窗口与地图配置
// ============================================
#define WIN_WIDTH	900	// 游戏窗口宽度
#define WIN_HEIGHT	600	// 游戏窗口高度
#define MAP_ROW  5		// 草坪行数
#define MAP_COL	 9		// 草坪列数

// 格子尺寸
#define GRID_WIDTH	80	// 格子宽度
#define GRID_HEIGHT	100	// 格子高度

// 地图偏移
#define MAP_OFFSET_X	250	// 地图X偏移
#define MAP_OFFSET_Y	100	// 地图Y偏移

// ============================================
// 阳光系统配置
// ============================================
#define SUN_SLOT_X	262	// 阳光槽X坐标
#define SUN_SLOT_Y	0	// 阳光槽Y坐标
#define SUN_VALUE	25	// 每个阳光的价值

// 天空阳光生成间隔（帧数，60帧=1秒）
#define SKY_SUN_INTERVAL	100	// 第一次生成阳光的间隔
#define SKY_SUN_INTERVAL_MIN	400	// 后续生成间隔最小值（随机范围）
#define SKY_SUN_INTERVAL_MAX	600	// 后续生成间隔最大值（随机范围）

// 向日葵产阳光间隔（帧数）
#define SUNFLOWER_SUN_INTERVAL	400	// 向日葵产生阳光的间隔

// 阳光收集动画速度
#define SUN_COLLECT_SPEED	15.0f	// 阳光飞向阳光槽的速度（像素/帧）

// ============================================
// 僵尸系统配置
// ============================================
#define ZOMBIE_SPAWN_INTERVAL	600	// 僵尸生成间隔（帧数）
const int MAX_ZOMBIES = 20;

// 僵尸移动速度
#define ZOMBIE_MOVE_SPEED 0.5f // 僵尸行走速度

// 僵尸碰撞检测
#define ZOMBIE_MOUTH_OFFSET	20	// 僵尸嘴部相对于图片左边的偏移
#define ZOMBIE_COLLISION_OFFSET	-20	// 
#define ZOMBIE_WIDTH	170	// 僵尸图片宽度
// 僵尸碰撞检测偏移（正值增大检测范围）
#define ZOMBIE_EAT_INTERVAL	1000	// 僵尸吃植物伤害间隔（毫秒）

// 僵尸动画帧数
#define ZOMBIE_SPAWN_Y_OFFSET	-60	// 僵尸生成Y坐标偏移（负值向上）
#define ZOMBIE_WALK_FRAMES	22	// 行走动画帧数
#define ZOMBIE_EAT_FRAMES	21	// 吃植物动画帧数
#define ZOMBIE_FRAME_DURATION	100	// 动画帧时长（毫秒）

// ============================================
// 子弹系统配置
// ============================================
#define BULLET_SPEED	8	// 子弹移动速度
#define BULLET_DAMAGE	20	// 子弹伤害值

// 子弹发射位置偏移（相对于格子左上角）
#define BULLET_OFFSET_X	10	// 子弹X偏移（从格子左边算起）
#define BULLET_OFFSET_Y	-15	// 子弹Y偏移（从格子顶部算起）

// 子弹碰撞检测范围（相对于僵尸X坐标）
#define BULLET_HIT_RANGE	100	// 子弹能击中僵尸的距离

// 豌豆射手射击间隔（帧数）
#define PEASHOOTER_SHOOT_INTERVAL	90	// 豌豆射手发射子弹的间隔

// ============================================
// 阳光UI显示位置
// ============================================
#define SUN_UI_X_SINGLE	282	// 阳光数量个位数显示X坐标
#define SUN_UI_X_MULTI	276	// 阳光数量两位数及以上显示X坐标
#define SUN_UI_Y	72	// 阳光数量显示Y坐标

// ============================================
// 阳光球配置
// ============================================
#define SUN_FALL_SPEED	2.0f	// 阳光下落速度控制
#define SUN_GROUND_Y_BASE	200	// 阳光落地点基准Y坐标
#define SUN_GROUND_Y_INTERVAL	90	// 每行阳光落地点Y间隔

// ============================================
// 向日葵产阳光配置
// ============================================
#define FLOWER_SUN_OFFSET_X	10	// 向日葵产阳光X偏移（相对于植物）
#define FLOWER_SUN_OFFSET_Y	-20	// 向日葵产阳光Y偏移（相对于植物）
#define FLOWER_SUN_BEZIER_HEIGHT	150	// 贝塞尔曲线控制点高度
#define FLOWER_SUN_BEZIER_SPEED	0.02f	// 贝塞尔曲线飞行速度

// ============================================
// 外部图片资源声明
// ============================================
extern IMAGE imgBg;		// 背景图片
extern IMAGE imgBar;	// 植物卡片栏图片
extern IMAGE imgSunshineBall[29];	// 阳光的29帧图片
extern IMAGE imgZM[22];			// 僵尸行走图片数组
extern IMAGE imgZMDead[20];		// 僵尸死亡的图片数组
extern IMAGE imgZMEat[21];		// 僵尸吃植物的图片数组
extern IMAGE imgZmStand[11];	// 僵尸站立的图片数组
extern IMAGE imgBulletNormal;		// 正常情况下的子弹图片
extern IMAGE imgBullBlast[4];		// 爆炸情况下的子弹图片数组

// 植物动画帧数配置
const int SUNFLOWER_FRAME_COUNT = 18; // 向日葵的帧数
const int PEASHOOTER_FRAME_COUNT = 13; // 豌豆射手的帧数

extern IMAGE imgSunflowerFrames[SUNFLOWER_FRAME_COUNT]; // 向日葵动画帧
extern IMAGE imgPeashooterFrames[PEASHOOTER_FRAME_COUNT]; // 豌豆射手动画帧

// ============================================
// 函数声明
// ============================================
bool fileExists(const std::string& filename);		// 判断文件是否存在
void loadImages();									// 加载植物摇摆动画图片



