#include <iomanip>
#include "city.h"
#include "game.h"

#define DEBUG 0 

using namespace std;

City::City(){
}
City::City(int number){
	if(number < 0)	this->number = 0;
	else			this->number = number;
}
void City::setCityNum(int number){
	if(number < 0)	this->number = 0;
	else			this->number = number;
}



NormalCity::NormalCity(void){
	redWarrior = NULL;
	blueWarrior = NULL;
}
int NormalCity::fight(Warrior *red, Warrior *blue){
	int ret1=0,ret2=0;
	Warrior *first,*last;	 
	if(number % 2){
		first = red;							//奇数城市，红方先出手 
		last = blue;
	}else{
		first = blue;
		last = red;
	}
	first->weaponSort();
	last->weaponSort();

	//当某一方只剩武器 剑 时，且它的剑攻击力为0，attack函数也返回0，退出循环 
	while(true){
		ret1 = first->attack(last);
		if(!first->alive() || !last->alive())	//出现死人的情况，则退出循环  	
			break;
		if(ret1 && ret2)						//双方都没有武器，则返回0
			return 0;
		ret2 = last->attack(first);
		if(!first->alive() || !last->alive())	//出现死人的情况，则退出循环 
			break;
		if(ret1 && ret2)						//双方都没有武器，则返回0
			return 0;
	}
	if(red->alive() && blue->alive())			//都或者，返回0 
		return 0;
	if(red->alive())	{						//红方活着返回1 
		//cout<<"red alive"<<endl;
		return 1;	
	}
	else if(blue->alive())	{					//蓝方活着返回-1 
		//cout<<"blue alive"<<endl;
		return -1;	
	}
	else										//都死了返回 -2 
		return -2;
}


Headquarter::Headquarter(int number, string team, int elements, int loyaltyDec):City(number){
	this->team = team;
	this->elements = elements;
	this->warriorNum = 0;
	this->setWarriorOrder(team);
	this->loyaltyDec = loyaltyDec;
	this->warrior = NULL;
}
void Headquarter::setWarriorOrder(string team){
	if(0 == team.compare("blue")){
		warriorOrder[0] = "lion";
		warriorOrder[1] = "dragon";
		warriorOrder[2] = "ninja";
		warriorOrder[3] = "iceman";
		warriorOrder[4] = "wolf";
	}else if(0 == team.compare("red")){
		warriorOrder[0] = "iceman";
		warriorOrder[1] = "lion";
		warriorOrder[2] = "wolf";
		warriorOrder[3] = "ninja";
		warriorOrder[4] = "dragon";
	}
}
string Headquarter::getWarriorName(void){
	return warriorOrder[warriorNum%5];
} 
int Headquarter::decElements(int elements){
	if(this->elements >= elements){
		this->elements -= elements;
		return 0;
	}
	else
		return -1;
}
Warrior * Headquarter::makeWarrior(int elements, int force, int totalCities){
	bool direction;
	if(this->elements < elements)							//基地生命元过低时，无法生产武士 
		return NULL;

	if(0 == team.compare("red"))	direction = true;	 
	else							direction = false;

	if(0 == warriorOrder[warriorNum%5].compare("dragon")){
		warrior = new Dragon(team, ++warriorNum, elements, force, number, totalCities, direction);
	}	
	else if(0 == warriorOrder[warriorNum%5].compare("ninja")){
		warrior = new Ninja(team, ++warriorNum, elements, force, number, totalCities, direction);
	}	
	else if(0 == warriorOrder[warriorNum%5].compare("iceman")){
		warrior = new Iceman(team, ++warriorNum, elements, force, number, totalCities, direction);
	}
	else if(0 == warriorOrder[warriorNum%5].compare("lion")){
		warrior = new Lion(team, ++warriorNum, elements, force, number, totalCities, direction, this->elements-elements, loyaltyDec);
		Lion *lion = dynamic_cast<Lion *>(warrior);
		if(lion->loyalty <= 0){		//如果刚好创建狮子后，基地血量为0，则狮子忠诚度也为0，狮子逃跑 
			lion->escape();
			delete warrior;
			warrior = NULL;
		}	
	}
	else if(0 == warriorOrder[warriorNum%5].compare("wolf")){
		warrior = new Wolf(team, ++warriorNum, elements, force, number, totalCities, direction);
	}
	decElements(elements);				//制造武士后，基地生命元降低 
	if(warrior)
		warriorVec.push_back(warrior);	//如果狮子没有逃跑，则加入该阵营的武士容器 
	return warrior;	
}
int Headquarter::eraseWarrior(Warrior *warrior){
	for(vector<Warrior *>::iterator  i=warriorVec.begin(); i!=warriorVec.end(); ++i){
		if((*i) == warrior){
			warriorVec.erase(i);
			delete warrior;
			return 0;
		}
	}
	return -1;
}
void Headquarter::report(void){
	//008:50 5520 elements in red headquarter 
	Game::printGameTime(); 
	cout<<elements<<" elements in "<<team<<" headquarter"<<endl;
} 
void Headquarter::showWarrior(void){
	for(vector<Warrior *>::const_iterator i=warriorVec.begin(); i!=warriorVec.end(); ++i){
		cout<<"team="<<(*i)->team<<",name="<<(*i)->name<<",number="<<(*i)->number<<",elements="\
		<<(*i)->elements<<",force="<<(*i)->force<<",city="<<(*i)->city<<",totalCities="<<(*i)->totalCities<<",direction="<<(*i)->direction<<endl;
	
		if(0 == (*i)->name.compare("lion")){
			Lion *lion = dynamic_cast<Lion *>(*i);
			cout<<" Loyalty="<<lion->loyalty<<" loyaltyDec="<<loyaltyDec;
		}
		cout<<endl;
	}
}	
void Headquarter::showCity(){
	cout<<team<<", cityNumber="<<number<<", baseName="<<team<<", elements="<<elements<<", warriorTotalNum="<<warriorNum<<endl;
} 

#if DEBUG
int main(){
	//测试1 创建普通城市
//	int MAX = 10;
//	NormalCity city[MAX];
//	for(int i=0;i<MAX; i++){
//		city[i].setCityNum(i+1);
//		cout<<"create normal city, "<<"Number="<<city[i].number<<endl;
//	} 

	//测试2 创建红蓝基地
//	Headquarter *red = new Headquarter(0, "red", 1000, 100);
//	Headquarter *blue = new Headquarter(11, "blue", 2000, 70);
//	red->showCity();
//	blue->showCity();
//
//	//测试3 生产武士
//	Warrior *w1 = red->makeWarrior(100, 20);
//	Warrior *w2 = red->makeWarrior(200, 50);
//	Warrior *w3 = red->makeWarrior(150, 32);
//	
////	red->makeWarrior(147, 21);
////	red->makeWarrior(23, 501);
////	red->makeWarrior(44, 10);
////	red->makeWarrior(30, 88);	
//
//	red->showWarrior();
//	cout<<"after"<<endl;
////	//red->showCity();
//	int ret = red->eraseWarrior(w1);
////
//	if(ret == 0){
//		cout<<w1->team<<" "<<w1->name<<" "<<w1->number <<" elements="<<w1->elements<<" force="<<w1->force;
//		delete w1;
//	}
////		
//	
//	red->showWarrior();
	
	
	//测试4 两个武士战斗
//	int ret=0;
//	city[0].redWarrior = red->makeWarrior(100, 20);
//	city[0].blueWarrior = blue->makeWarrior(200, 50);
//	
//	city[0].redWarrior->showWeaponInfo();
//	city[0].redWarrior->showTestInfo();
//	city[0].blueWarrior->showWeaponInfo();
//	city[0].blueWarrior->showTestInfo();
//
//	ret = city[0].fight(city[0].redWarrior, city[0].blueWarrior);
//	if(ret == -2)
//		cout<<"both died"<<endl;
//	else if(ret == -1)
//		cout<<"blue died"<<endl;
//	else if(ret == 1)
//		cout<<"red died"<<endl;
//	else if(ret == 0)
//		cout<<"both alive"<<endl;
}
#endif
