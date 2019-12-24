#ifndef __WEAPON__H
#define __WEAPON__H

#include <iostream>

using namespace std;

//基类：武器
class Weapon{
public:
	int force;							//攻击力
	int times;							//剩余使用次数
	int number; 						//武器的编号 
public:
	Weapon(int number, int force);
	int resetForce(int force);			//武器更换主人，重新计算攻击力
	virtual int useWeapon(void);
};

//派生类：剑
class Sword : public Weapon{
public:	
	Sword(int force);
};

//派生类：炸弹 
class Bomb : public Weapon{
public: 
	Bomb(int force);
	int useWeapon(void);			//使用武器后，返回剩余可使用次数 
};

//派生类：弓箭 
class Arrow : public Weapon{
public: 	
	Arrow(int force);
	int useWeapon(void);			//使用武器后，返回剩余可使用次数 
};

#endif
