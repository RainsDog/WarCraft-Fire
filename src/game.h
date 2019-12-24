#ifndef __GAME__H
#define __GAME__H

#include <iostream>
#include <map>
#include "city.h" 

#define WARRIOR_KIND	5 
     
typedef struct WARRIOR_INFO{
	string name;
	int element;
	int force;
}warriorInfo_t;

typedef struct GAME_INFO{
	int testCase;
	int initElements;
	int totalCities;
	int decLoyalty;
	int totalTime;
	warriorInfo_t warrior[WARRIOR_KIND];
}gameInfo_t;

class Game{
public:
	int initElements;			//基地初始生命元
	int decLoyalty;				//狮子每次递减的忠诚度 
	int totalCities;			//城市总数量（不算红蓝军基地）
	int totalTime;				//总的游戏时长，到时间后游戏结束 
	static int curTime;			//当前游戏时间
	warriorInfo_t *warrior;		//保存武士的初始化信息
public:
	Game(int initElements, int totalCities, int decLoyalty, int totalTime, warriorInfo_t *warrior); 	//创建游戏对象，游戏初始化 
	void begin(void);			//游戏正式开始 
	static void printGameTime(void);		//打印当前游戏时间
	bool checkGameTime(void); 				//检查游戏时间，确认是否应该结束游戏 
};


#endif
