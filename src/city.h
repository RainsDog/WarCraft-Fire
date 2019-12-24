#ifndef __CITY__H
#define __CITY__H

#include <iostream>
#include <vector>
#include "warrior.h"
using namespace std;

//基类：城市 
class City{
public:
	int number;								//每个城市都有编号
public:
	City();
	City(int number);
	void setCityNum(int number);		 
};

//司令部 
class Headquarter : public City{
public:
	string team;							//阵营名字
	int elements;							//基地剩余生命元
	int warriorNum; 						//武士最大编号
	int initLoyalty;						//狮子的初始化忠诚度 
	int loyaltyDec;							//狮子的忠诚度递减数 
	string warriorOrder[5];					//制造顺序  
	vector<Warrior *> warriorVec;			//用以存储的武士对象的指针
	Warrior *warrior;
public:
	Headquarter(int number, string team, int elements, int loyaltyDec);
	void setWarriorOrder(string team);		//设置阵容制造武士的顺序
	string getWarriorName(void);			//获取当前需要制造的武士名字 
	int decElements(int elements);			//剩余生命源数量减少
	Warrior * makeWarrior(int elements, int force, int totalCities);	//制造武士
	int eraseWarrior(Warrior *warrior);		//删除死去的武士 
	void report(void);						//司令部报告剩余生命元 
	void showWarrior(void);					//显示目前阵营所有活着的武士，仅供测试用
	void showCity(void);					//显示城市信息 ，仅供测试用
};

//普通城市类 
class NormalCity : public City{
public:
	Warrior *redWarrior;
	Warrior *blueWarrior;
public:
	NormalCity();
	int fight(Warrior *red, Warrior *blue);
};




#endif
