#include "weapon.h"
#define DEBUG 0

using namespace std;

//基类 
Weapon::Weapon(int number, int force){
	//判断初始化数据是否合理 
	if(number < 0 || number > 2)  	this->number = 0;
	else							this->number = number;
	if(force < 0)	this->force = 0;
	else			this->force = force;
}
int Weapon::resetForce(int force){
	if(force < 0)	this->force = 0;
	else			this->force = force;
	
}
int Weapon::useWeapon(void){return times;}				//不降低可使用次数 

//剑 
Sword::Sword(int force):Weapon(0, force){times = 1;}	//剑的可使用次数初始化为1

//炸弹 
Bomb::Bomb(int force):Weapon(1, force){times = 1;}		//炸弹的可使用次数初始化为1 
int Bomb::useWeapon(void){								//使用炸弹，降低可使用次数，返回剩余可使用次数 
	if(times > 0)		times--;
	return times;
}

//弓箭 
Arrow::Arrow(int force):Weapon(2, force){times = 2;}	//弓箭的可使用次数初始化为2
int Arrow::useWeapon(void){								//使用弓箭，降低可使用次数，返回剩余可使用次数	
	if(times > 0)		times--;
	return times;
} 

#if DEBUG 
int main(){
	//test1 测试构造函数 
	Sword b(35);
	Bomb  c(48);
	Arrow d(51);
	cout<<"b number="<<b.number<<",force="<<b.force<<",times="<<b.times<<endl;
	cout<<"c number="<<c.number<<",force="<<c.force<<",times="<<c.times<<endl;
	cout<<"d number="<<d.number<<",force="<<d.force<<",times="<<d.times<<endl;
	cout<<endl;

	//test2 测试其它函数
	b.resetForce(200);
	c.resetForce(300);
	d.resetForce(400);
	cout<<"b number="<<b.number<<",force="<<b.force<<",times="<<b.times<<endl;
	cout<<"c number="<<c.number<<",force="<<c.force<<",times="<<c.times<<endl;
	cout<<"d number="<<d.number<<",force="<<d.force<<",times="<<d.times<<endl;
	cout<<endl;
	
	int bt, ct, dt;
	bt = b.useWeapon();
	ct = c.useWeapon();
	dt = d.useWeapon();
	cout<<"b bt="<<bt<<",force="<<b.force<<",times="<<b.times<<endl;
	cout<<"c ct="<<ct<<",force="<<c.force<<",times="<<c.times<<endl;
	cout<<"d dt="<<dt<<",force="<<d.force<<",times="<<d.times<<endl;
	cout<<endl;
	bt = b.useWeapon();
	ct = c.useWeapon();
	dt = d.useWeapon();
	cout<<"b bt="<<bt<<",force="<<b.force<<",times="<<b.times<<endl;
	cout<<"c ct="<<ct<<",force="<<c.force<<",times="<<c.times<<endl;
	cout<<"d dt="<<dt<<",force="<<d.force<<",times="<<d.times<<endl;
}
#endif


