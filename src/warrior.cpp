#include <iomanip>
#include "warrior.h" 
#include "game.h"
#define DEBUG 0

using namespace std;

//武器进行排序时的算法 
bool mySort(const WEAPON_PAIR& lhs, const WEAPON_PAIR& rhs)
{
	if(lhs.first != rhs.first)				//编号不相同的武器，根据编号排序 
    	return lhs.first < rhs.first;
    else									//编号相同的武器，根据剩余使用次数排序	
    	return lhs.second->times < rhs.second->times;
}
 
Warrior::Warrior(){
	this->name 		= "default";
	this->team  	= "default";
	this->number 	= 0;
	this->elements 	= 0;
	this->force 	= 0;
	this->city 		= 0;
	this->totalCities= 0; 
	this->direction	= false;
	Game::printGameTime();
	printBornInfo();
}
	
Warrior::Warrior(string name, string team, int number, int elements, int force, int city, int totalCities, bool direction){
	if(name.empty())	this->name 		= "default";
	else				this->name 		= name;

	if(team.empty())	this->team 		= "default";
	else				this->team 		= team;
	
	if(number < 0)		this->number 	= 0;
	else				this->number 	= number;
	
	if(elements < 0)	this->elements 	= 0;
	else				this->elements 	= elements;
	
	if(force < 0)		this->force 	= 0;
	else				this->force 	= force;

	if(city < 0)		this->city 		= 0;
	else				this->city 		= city;
	
	if(totalCities < 0)	this->totalCities	= 0;
	else				this->totalCities	= totalCities;
	
	this->direction = direction;
	Game::printGameTime(); 
	printBornInfo();
}
/*
一般情况下，武士消亡时，武器会被敌人缴获
但是当敌人武器数量等于10时，武器并不会被全部缴获，因此需要手动删除 
*/
Warrior::~Warrior(){				//武士消亡时，武器库也要清空 
	//cout<<"~Warrior call"<<endl;
	for(WEAPON_VEC::iterator i=weaponVec.begin(); i!=weaponVec.end(); ++i){
		delete ((*i).second);		//删除武器库的武器对象 
	} 
	weaponVec.clear(); 				//清空容器
}
int Warrior::decElements(int force){
	if(force <= 0)
		return -2; 			//攻击为负，或者攻击为0，返回-2
	if(elements > 0){
		elements -= force;	//正常攻击，减血，返回0 
		return 0;
	}
	return -1;				//血量已经不大于0，返回-1 
}
bool Warrior::alive(void){
	if(elements > 0)
		return true;
	else
		return false;
}
int Warrior::march(NormalCity* iCity){
	int enemyCity = 0; 
	if(0 == team.compare("red")){
		enemyCity = totalCities+1;
	}else if(0 == team.compare("blue")){
		enemyCity = 0;
	}else{
		return -2;			//未知错误 
	}
	if(city == enemyCity)	//如果当前已经在敌方基地，则无需前进，返回-1
		return -1;
	//前进一个城市 
	if(direction)	city += 1;		
	else			city -= 1;
	//cout<<team<<" "<<name<<", number="<<number<<" march city = "<<city<<", enemyCity = "<<enemyCity<<endl;
	setCity(iCity); 
	//到达敌方基地，返回0, 到达常规城市，返回1 
	if(city == enemyCity)		return 0;
	else						return 1;					
}
void Warrior::setCity(NormalCity* iCity){		//武士前进后，需要将之前和当前的城市对象中的武士指针重新赋值 
	if(totalCities < 0 || NULL == iCity)		//参数不对直接退出 
		return;
	if(0 == team.compare("red")){
		if((city - 1) > 0){
			iCity[city - 2].redWarrior = NULL;	//武士前进后，上一个城市不为基地，则赋值为NULL
		}else{
			//cout<<"no null "<<team<<" "<<name<<" number="<<number<<" city="<<city<<endl;
		}
		if(city != (totalCities+1)){				//武士前进后，所在城市不是敌方基地 				
			iCity[city - 1].redWarrior = this;	//将武士的指针赋值给城市对象 
		}else{
			//cout<<"no this "<<team<<" "<<name<<" number="<<number<<" city="<<city<<endl;
		}
	}else if(0 == team.compare("blue")){
		if((city + 1) < (totalCities+1)){
			iCity[city].blueWarrior = NULL;		//武士前进后，上一个城市不为基地，则赋值为NULL
		}
		if(city != 0){							//武士前进后，所在城市不是敌方基地 
			iCity[city-1].blueWarrior = this;	//将武士的指针赋值给城市对象
		}
	}
}
int Warrior::attack(Warrior * enemy){
	if((NULL == enemy) || (enemy == this))
		return -1;										//传入错误参数，直接返回-1 
	if(weaponVec.empty())
		return 1;										//没有武器时，直接返回1
	if(checkWeaponNoForce())							//武器为剑，且没有伤害时，返回 
		return 1;
	int number = (*iweapon).second->number;				//获取当前武器的编号 
	int force  = (*iweapon).second->force;				//获取当前武器的攻击力
	int times  = (*iweapon).second->useWeapon();		//使用武器，获取剩余可使用次数 
	enemy->decElements(force);							//敌人血量降低 
	//cout<<team<<" "<<name<<" attack "<<enemy->team<<" "<<enemy->name<<" with weapon number "<<number<<", enemy elements dec "<<force<<", now elements is "<<elements<<" enemy elements is "<<enemy->elements<<endl;
	if(1 == number){ 									//使用炸弹时 
		this->decElements(force/2);						//自己受到伤害的一半
		//cout<<team<<" "<<name<<"get hurt form bomb, elements dec "<<force/2<<endl; 
	} 
	if(times > 0)										//如果武器还能用 
		iweapon++;										//则指向下一个武器 
	else{
		delete (*iweapon).second;						//删除武器对象
		weaponVec.erase(iweapon);						//从容器中移除 
	} 
	if(iweapon == weaponVec.end())
		iweapon = weaponVec.begin();					//武器用完一遍又回到开头
	return 0;
}
void Warrior::weaponSort(void){
	sort(weaponVec.begin(), weaponVec.end(), mySort);
	iweapon = weaponVec.begin();						//指针指向武器库的开头 
}
void Warrior::getEnemyWeapon(Warrior * enemy){
	if((NULL == enemy) || (enemy == this))
		return;											//传入错误参数，直接返回 	
	if(enemy->weaponVec.empty())						//如果敌人没有武器，则直接返回 
		return;
	for(WEAPON_VEC::iterator i=enemy->weaponVec.begin(); i!=enemy->weaponVec.end(); ){
		if(weaponVec.size() < 10){						//武器少于10个，才缴获 
			if(0 == (*i).first){						//攻击力重新计算 
				(*i).second->force = force*2/10;
			}else if(1 == (*i).first){
				(*i).second->force = force*4/10;
			}else if(2 == (*i).first){
				(*i).second->force = force*3/10;
			}
			weaponVec.push_back(*i);					//放入武器容器中 
			enemy->weaponVec.erase(i);					//移除原先武士的对应武器
		}else
			break;										//武器达到10个，直接退出 
	}
	weaponSort();										//缴获武器后，进行排序
}
bool Warrior::checkWeaponNoForce(void){
	for(WEAPON_VEC::const_iterator i=weaponVec.begin(); i!=weaponVec.end(); ++i){
		if((*i).second->force > 0){			//武器有攻击力，则武器正常使用	
			return false;
		} 				 	
		if((*i).first != 0){				//攻击力为0的炸弹和弓箭，也要使用
			return false;
		}		
	}
	return true;
}
void Warrior::showInfo(void){
	int swordNum = 0;
	int bombNum = 0;
	int arrowNum = 0;
	for(WEAPON_VEC::const_iterator i=weaponVec.begin(); i!=weaponVec.end(); ++i){
		if(0 == (*i).first)
			swordNum++;
		else if(1 == (*i).first)
			bombNum++;
		else if(2 == (*i).first)
			arrowNum++;
	}
	Game::printGameTime(); 
	cout<<team<<" "<<name<<" "<<number <<" has "<<swordNum<<" sword "<<bombNum<<" bomb "<<arrowNum<<" arrow and "<<elements<<" elements"<<endl;
} 
void Warrior::showWeaponInfo(void){
	cout<<team<<" "<<name<<" "<<number<<" has "<<endl;
	for(WEAPON_VEC::const_iterator i=weaponVec.begin(); i!=weaponVec.end(); ++i){
		cout<<"number="<<(*i).second->number<<", force="<<(*i).second->force<<",times="<<(*i).second->times<<endl;
	}
	if(weaponVec.empty())
		cout<<"nothing"<<endl;
}

void Warrior::showTestInfo(void){
	cout<<"name="<<name<<",team="<<team<<",number="<<number<<",elements="\
		<<elements<<",force="<<force<<",city="<<city<<",totalCities="<<totalCities<<",direction="<<direction<<endl;
}
void Warrior::printBornInfo(void){
	cout<<team<<" "<<name<<" "<<number<<" born"<<endl;
}



Dragon::Dragon(string team, int number, int elements, int force, int city, int totalCities, bool direction):Warrior("dragon", team, number, elements, force, city, totalCities, direction){
	//龙诞生时，获得武器 
	if(0 == number%3)
		weaponVec.push_back(make_pair(0, new Sword(force*2/10)));
	else if(1 == number%3)
		weaponVec.push_back(make_pair(1, new Bomb(force*4/10)));	
	else
		weaponVec.push_back(make_pair(2, new Arrow(force*3/10)));	
}
Dragon::~Dragon(){
	//cout<<"Dragon call"<<endl; 
}
void Dragon::yelled(void){
	//red dragon 5 yelled in city 7
	Game::printGameTime(); 
	cout<<team<<" "<<name<<" "<<number<<" yelled in city "<<city<<endl;
}




Ninja::Ninja(string team, int number, int elements, int force, int city, int totalCities, bool direction):Warrior("ninja", team, number, elements, force, city, totalCities, direction){
	if(0 == number%3){
		weaponVec.push_back(make_pair(0, new Sword(force*2/10)));
		weaponVec.push_back(make_pair(1, new Bomb(force*4/10)));
	}
	else if(1 == number%3){
		weaponVec.push_back(make_pair(1, new Bomb(force*4/10)));
		weaponVec.push_back(make_pair(2, new Arrow(force*3/10)));		
	}	
	else{
		weaponVec.push_back(make_pair(2, new Arrow(force*3/10)));
		weaponVec.push_back(make_pair(0, new Sword(force*2/10)));	
	}	
}
Ninja::~Ninja(){
	//cout<<"Ninja call"<<endl; 
}
int Ninja::attack(Warrior * enemy){
	if((NULL == enemy) || (enemy == this))
		return -1;										//传入错误参数，直接返回-1 
	if(weaponVec.empty())
		return 1;										//没有武器时，直接返回1
	if(checkWeaponNoForce())							//武器为剑，且没有伤害时，返回 
		return 1;

	int number = (*iweapon).second->number;				//获取当前武器的编号 
	int force  = (*iweapon).second->force;				//获取当前武器的攻击力
	int times  = (*iweapon).second->useWeapon();		//使用武器，获取剩余可使用次数 

	enemy->decElements(force);							//敌人血量降低 
	//cout<<name<<" attack "<<enemy->name<<" with weapon number "<<number<<", enemy elements dec "<<force<<endl;

	//忍者使用炸弹不受伤害 
	if(1 == number){ 									//使用炸弹时 
		//this->decElements(force/2);						//自己受到伤害的一半
		//cout<<name<<"get hurt form bomb, elements dec "<<force/2<<endl; 
		//cout<<"Ninja protected himself from being hurt by bomb"<<endl;
	} 

	if(times > 0)										//如果武器还能用 
		iweapon++;										//则指向下一个武器 
	else{
		delete (*iweapon).second;						//删除武器对象
		weaponVec.erase(iweapon);						//从容器中移除 
	} 
	
	if(iweapon == weaponVec.end())
		iweapon = weaponVec.begin();					//武器用完一遍又回到开头
	return 0;
}


Iceman::Iceman(string team, int number, int elements, int force, int city, int totalCities, bool direction):Warrior("iceman", team, number, elements, force, city, totalCities, direction){
	if(0 == number%3)
		weaponVec.push_back(make_pair(0, new Sword(force*2/10)));
	else if(1 == number%3)
		weaponVec.push_back(make_pair(1, new Bomb(force*4/10)));	
	else
		weaponVec.push_back(make_pair(2, new Arrow(force*3/10)));		
}
Iceman::~Iceman(){
	//cout<<"Iceman call"<<endl; 
}
int Iceman::march(NormalCity* iCity){
	int enemyCity = 0; 
	if(0 == team.compare("red")){
		enemyCity = totalCities+1;
	}else if(0 == team.compare("blue")){
		enemyCity = 0;
	}else{
		return -2;			//未知错误 
	}
	
	if(city == enemyCity)	//如果当前已经在敌方基地，则无需前进，返回-1
		return -1;
	
	//前进一个城市 
	if(direction)	city += 1;		
	else			city -= 1;
	//cout<<team<<" "<<name<<", number="<<number<<" march city = "<<city<<", enemyCity = "<<enemyCity<<endl;
	setCity(iCity); 
	elements -= elements/10;		//每前进一个城市，血量减少1/10
	//到达敌方基地，返回0, 到达常规城市，返回1 
	if(city == enemyCity)		return 0;
	else						return 1;	
}




Lion::Lion(string team, int number, int elements, int force, int city, int totalCities, bool direction, int loyalty, int loyaltyDec):Warrior("lion", team, number, elements, force, city, totalCities, direction){
	if(0 == number%3)
		weaponVec.push_back(make_pair(0, new Sword(force*2/10)));
	else if(1 == number%3)
		weaponVec.push_back(make_pair(1, new Bomb(force*4/10)));	
	else
		weaponVec.push_back(make_pair(2, new Arrow(force*3/10)));
	this->loyalty = loyalty;		
	this->loyaltyDec = loyaltyDec;		
	printLoyalty();
}
Lion::~Lion(){
	//cout<<"Lion call"<<endl; 
}
int Lion::march(NormalCity* iCity){
	int enemyCity = 0; 
	if(0 == team.compare("red")){
		enemyCity = totalCities+1;
	}else if(0 == team.compare("blue")){
		enemyCity = 0;
	}else{
		return -2;			//未知错误 
	}
	
	if(city == enemyCity)	//如果当前已经在敌方基地，则无需前进，返回-1
		return -1;
	
	//前进一个城市 
	if(direction)	city += 1;		
	else			city -= 1;
	//cout<<team<<" "<<name<<", number="<<number<<" march city = "<<city<<", enemyCity = "<<enemyCity<<endl;
	setCity(iCity); 
	loyalty -= loyaltyDec;		//每前进一个城市，忠诚度降低 
	//到达敌方基地，返回0, 到达常规城市，返回1 
	if(city == enemyCity)		return 0;
	else						return 1;	
}
void Lion::escape(void){
	//002:05 blue lion 1 ran away
	Game::printGameTime(); 
	cout<<team<<" "<<name<<" "<<number<<" ran away"<<endl; 
}
void Lion::showTestInfo(void){
	cout<<"name="<<name<<",team="<<team<<",number="<<number<<",elements="\
		<<elements<<",force="<<force<<",city="<<city<<",direction="<<direction<<",loyalty="<<loyalty<<",loyaltyDec="<<loyaltyDec<<endl;
}
void Lion::printLoyalty(void){
	cout<<"Its loyalty is "<<loyalty<<endl;
}



Wolf::Wolf(string team, int number, int elements, int force, int city, int totalCities, bool direction):Warrior("wolf", team, number, elements, force, city, totalCities, direction){}
Wolf::~Wolf(){
	//cout<<"Wolf call"<<endl; 
}
int Wolf::rob(Warrior * enemy){
	if((NULL == enemy) || (enemy == this))
		return -1;					//传入错误参数，直接返回-1 
	if(enemy->weaponVec.empty() || (0 == enemy->name.compare("wolf")))
		return 1;					//敌人没有武器或者敌人是狼时，直接返回1
		
	int swordNum = 0;
  	int bombNum = 0;
	int arrowNum = 0;
				
	enemy->weaponSort();			//将敌人武器分类后，从编号小的开始抢																		
	int  min = (*(enemy->weaponVec.begin())).first;		//记下敌人最小的武器编号 
	for(WEAPON_VEC::iterator i=enemy->weaponVec.begin(); i!=enemy->weaponVec.end(); ){
		if((*i).first != min)							//抢完所有最小编号的武最后就退出 
			break; 
		if(weaponVec.size() < 10){						//狼的武器少于10个，才抢 
			if(0 == (*i).first){						//攻击力重新计算 
				swordNum++;
				(*i).second->force = force*2/10;
			}else if(1 == (*i).first){
				bombNum++;
				(*i).second->force = force*4/10;
			}else if(2 == (*i).first){
				arrowNum++;
				(*i).second->force = force*3/10;
			}
			weaponVec.push_back(*i);					//放入狼的武器容器中 
			enemy->weaponVec.erase(i);					//移除原先武士的对应武器
		}	
	}
	//008:35 red wolf 3 took 1 sword from blue lion 6 in city 7
	Game::printGameTime(); 
	cout<<team<<" "<<name<<" "<<number<<" took ";
	
	if(swordNum || bombNum || arrowNum){
		if(swordNum){
			cout<<swordNum<<" sword ";
		}
		if(bombNum){
			cout<<bombNum<<" bomb ";
		}
		if(arrowNum){
			cout<<arrowNum<<" arrow ";
		}
	}
	cout<<"from "<<enemy->team<<" "<<enemy->name<<" "<<enemy->number<<" in city "<<city<<endl;
	return 0;
}
#if DEBUG
int main(){
	//测试1 : 武士基类的构造函数和所有信息打印函数 
//	Warrior *w1 = new Warrior(); 
//	Warrior *w2 = new Warrior("pig", "blue", 10, 100, 120, 3, false); 
// 
//	w1->showInfo();
//	w2->showInfo();
//	
//	w1->showWeaponInfo();
//	w2->showWeaponInfo();
//	
//	w1->showTestInfo();
//	w2->showTestInfo();
	
	//测试2 : 武士血量降低，判断武士是否活着
//	Warrior *w2 = new Warrior("pig", "blue", 10, 100, 120, 3, false); 
//	if(w2->alive())
//		cout<<"before:alive"<<endl;
//	else
//		cout<<"before:died"<<endl;
	//血量降低为0的测试 
//	w2->decElements(100);
//	if(w2->alive())
//		cout<<"after:alive"<<endl;
//	else
//		cout<<"after:died"<<endl; 
	//血量降低为负数的测试 
//	w2->decElements(200);
//	if(w2->alive())
//		cout<<"after:alive"<<endl;
//	else
//		cout<<"after:died"<<endl;  
	
	//测试3 : 武士前进到下一个城市
//	int ret=1;
//	Warrior *w2 = new Warrior("pig", "blue", 10, 100, 120, 4, false); 
//	w2->showTestInfo(); 
//	while(ret > 0){				//返回值大于0，说明之前前进到一个普通城市，可以进行下一次前进 
//		ret = w2->march(0);	
//	}
	
	//测试4 ：武器排序、攻击、和缴获测试 
//	Warrior *w1 = new Warrior("redBoy", "red",  10,  500, 120, 3, true); 
//	Warrior *w2 = new Warrior("blueGirl", "blue",  8,  200, 60,  0, false);
//	 
//	//创建武器对象 
//	Weapon *pSword1  = new Sword(10);
//	Weapon *pSword2  = new Sword(20);
//	Weapon *pSword3  = new Sword(30);
//	Weapon *pBomb1   = new Bomb(40);
//	Weapon *pBomb2   = new Bomb(50);
//	Weapon *pBomb3   = new Bomb(60);
//	Weapon *pArrow1  = new Arrow(70);
//	Weapon *pArrow2  = new Arrow(80);
//	Weapon *pArrow3  = new Arrow(90);
//	Weapon *pArrow4  = new Arrow(80);
//	Weapon *pArrow5  = new Arrow(70);
//	
//	//给双方发武器
//	w1->weaponVec.push_back(make_pair(pArrow1->number, pArrow1));
//	w1->weaponVec.push_back(make_pair(pSword1->number, pSword1));
//	w1->weaponVec.push_back(make_pair(pBomb2->number, pBomb2));
//	w1->weaponVec.push_back(make_pair(pArrow2->number, pArrow2));
//	
//	w2->weaponVec.push_back(make_pair(pArrow3->number, pArrow3));
//	w2->weaponVec.push_back(make_pair(pSword2->number, pSword2));
//	w2->weaponVec.push_back(make_pair(pSword3->number, pSword3));
//	w2->weaponVec.push_back(make_pair(pBomb1->number, pBomb1));
//	w2->weaponVec.push_back(make_pair(pBomb3->number, pBomb3));
//	w2->weaponVec.push_back(make_pair(pArrow4->number, pArrow4));
//	w2->weaponVec.push_back(make_pair(pArrow5->number, pArrow5));
	
	//展示武器数量 
//	w1->showInfo();
//	w2->showInfo();
//	w1->showWeaponInfo();
//	w2->showWeaponInfo();
//	
//	排序函数
//	pArrow2->useWeapon();	//使用一次，看排序是否正确 
//	pArrow4->useWeapon();	//使用一次，看排序是否正确 
//	w1->weaponSort();
//	w2->weaponSort();
//	cout<<"after sort"<<endl; 
//	w1->showWeaponInfo();
//	w2->showWeaponInfo();

	//缴获武器测试
//	cout<<"after take"<<endl; 
//	//w1->getEnemyWeapon(w2);
//	w2->getEnemyWeapon(w1);
//	w1->showWeaponInfo();
//	w2->showWeaponInfo();


	//单步攻击测试
//	w1->weaponSort();
//	w2->weaponSort();
//	w1->showWeaponInfo();
//	w2->showWeaponInfo();
//	
//	w1->showTestInfo(); 
//	w2->showTestInfo();
//	w1->attack(w2);
//	cout<<"attack time 1"<<endl;
//	w1->showTestInfo(); 
//	w2->showTestInfo(); 
//	w1->attack(w2);
//	cout<<"attack time 2"<<endl;
//	w1->showTestInfo(); 
//	w2->showTestInfo(); 
	
	//致死攻击测试
//	w1->weaponSort();
//	w2->weaponSort();
//	w1->showWeaponInfo();
//	w2->showWeaponInfo();
//	w1->showTestInfo(); 
//	w2->showTestInfo(); 
//	cout<<"before attack test"<<endl;
//	int ret1=0,ret2=0;
//	while(true){
//		ret1 = w1->attack(w2);
//		if(!w1->alive() || !w2->alive())	//出现死人的情况，则退出循环 
//			break;
//		if(ret1 && ret1)					//双方都没有武器，则退出循环 
//			break;
//			
//		ret2 = w2->attack(w1);
//		if(!w1->alive() || !w2->alive())	//出现死人的情况，则退出循环 
//			break;
//		if(ret1 && ret1)					//双方都没有武器，则退出循环 
//			break;
//	} 
//	w1->showWeaponInfo();
//	w2->showWeaponInfo();
//	w1->showTestInfo(); 
//	w2->showTestInfo(); 	



	//测试5 ：子类龙的测试 
//	Dragon *w1 = new Dragon("red", 12, 500, 100, 3, true);
//	w1->showWeaponInfo();
//	w1->yelled();

	//测试6 ：子类忍者的测试 
//	Dragon *w1 = new Dragon("red", 12, 500, 100, 3, true);
//	Warrior *w2 = new Ninja("blue", 12, 200, 200, 3, false);
//
//	w1->weaponSort();
//	w2->weaponSort();
//	
//	w1->showWeaponInfo();
//	w2->showWeaponInfo();
//	
//	w1->showTestInfo();
//	w2->showTestInfo();
//	
//	int ret1=0,ret2=0;
//	while(true){
//		ret2 = w2->attack(w1);
//		if(!w1->alive() || !w2->alive())	//出现死人的情况，则退出循环 
//			break;
//		if(ret1 && ret2)					//双方都没有武器，则退出循环 
//			break;
//		ret1 = w1->attack(w2);
//		if(!w1->alive() || !w2->alive())	//出现死人的情况，则退出循环 
//			break;
//		if(ret1 && ret2)					//双方都没有武器，则退出循环 
//			break;
//	} 
//	cout<<"after attack"<<endl;
//	w1->showTestInfo();
//	w2->showTestInfo();

	//测试7 ：子类冰人的测试 
//	Warrior *w1 = new Iceman("red", 12, 500, 100, 7, true);
//	w1->march(10);
//	w1->march(10);
//	w1->march(10);
//	w1->march(10);
	
	//测试8 ：子类狮子的测试 
//	Lion *w1 = new Lion("red", 12, 500, 100, 3, true, 100, 40);
//	for(int i=0; i<10; i++){
//		w1->march(10);
//		if(w1->loyalty <= 0)
//			w1->escape();
//	}

	//测试9 ：子类狼的测试 
//	Wolf *w3 = new Wolf("red", 12, 500, 100, 7, true);
//	
//	w1->showWeaponInfo();
//	w2->showWeaponInfo();
//	w3->rob(w1);
//	w1->showWeaponInfo(); 
//	w3->rob(w2);
//	w2->showWeaponInfo(); 
}
#endif 
