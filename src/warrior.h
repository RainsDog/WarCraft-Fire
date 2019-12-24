#ifndef __WARRIOR__H
#define __WARRIOR__H

#include <iostream>
#include <algorithm>
#include <vector>
#include "weapon.h"

using namespace std;
class Warrior;
class NormalCity;

//pair对象的first为武器编号，second为武器对象指针 
typedef pair<int,Weapon*>			WEAPON_PAIR;
typedef vector<WEAPON_PAIR> 		WEAPON_VEC;

//基类：武士 
class Warrior{
public:
	string name;	//武士名称			//"dragon", "ninja", "iceman", "lion", "wolf"
	string team;	//武士所属阵营 		//red，blue 
	int number;		//编号 				//按出生顺序编号 1-N 
	int elements;	//生命值
	int force;		//攻击力 
	int city;		//所在城市编号		//武士会路过不同的城市 
	int totalCities;//总城市数量 
	bool direction;	//行进方向 			//向东为true，向西为false
	WEAPON_VEC weaponVec;				//武士拥有的武器 
	WEAPON_VEC::iterator iweapon;	   	//轮流攻击时，依次遍历，指向下一个使用的武器 
public:
	Warrior(); 
	Warrior(string name, string team, int number, int elements, int force, int city, int totalCities, bool direction);
	virtual ~Warrior();
	int decElements(int force);				//受到攻击，减少生命值，若生命值已经不大于0，则返回-1 
	bool alive(void);						//返回武士是否存活的信息 
	virtual int march(NormalCity* iCity);	//前进，到下一个城市，参数是敌方基地的城市编号，否则不知道何时到达敌方基地
	void setCity(NormalCity* iCity);		//前进后，将自己的指针保存到城市对象中 
	virtual int attack(Warrior * enemy);	//攻击敌人
	void weaponSort(void);					//发动攻击前，对武器排序
	void getEnemyWeapon(Warrior * enemy);	//缴获敌人的武器
	bool checkWeaponNoForce(void);			//检查当前武器是不是无伤害，如果剑无伤害，则可能死循环 
	void showInfo(void);					//展示武器数量和血量 
	void showWeaponInfo(void);				//显示武器的具体信息，仅供调试使用
	virtual void showTestInfo(void);		//显示武士的具体信息，仅供调试使用
	void printBornInfo(void);				//打印出生信息 
};

//派生类：龙 
class Dragon : public Warrior{
public:
	Dragon(string team, int number, int elements, int force, int city, int totalCities, bool direction);
	~Dragon();
	void yelled(void);				//战胜后欢呼  
};

//派生类：忍者 
class Ninja : public Warrior{
public:
	Ninja(string team, int number, int elements, int force, int city, int totalCities, bool direction);
	~Ninja();
	int attack(Warrior * enemy);	//使用炸弹不受伤害 
};

//派生类：冰人 
class Iceman : public Warrior{
public:
	Iceman(string team, int number, int elements, int force, int city, int totalCities, bool direction);
	~Iceman();
	int march(NormalCity* iCity);
};

//派生类：狮子 
class Lion : public Warrior{
public:
	int loyalty;			//降生时的忠诚度 
	int loyaltyDec; 		//每次前进需要减少的忠诚度 
	Lion(string team, int number, int elements, int force, int city, int totalCities, bool direction, int loyalty, int loyaltyDec);
	~Lion();
	int march(NormalCity* iCity);
	void escape(void);		//狮子逃跑 
	void showTestInfo(void);
	void printLoyalty(void);
};

//派生类：狼
class Wolf : public Warrior{
public:
	Wolf(string team, int number, int elements, int force, int city, int totalCities, bool direction);
	~Wolf();
	int rob(Warrior * enemy);	//抢夺敌人最低编号的武器 
};

#endif
