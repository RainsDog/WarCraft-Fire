#include <iomanip>
#include "game.h"

#define DEBUG 0

using namespace std;

int Game::curTime = 0;

Game::Game(int initElements, int totalCities, int decLoyalty, int totalTime, warriorInfo_t *warrior){
	this->initElements 	= initElements;
	this->totalCities  	= totalCities;
	this->decLoyalty   	= decLoyalty;
	this->totalTime   	= totalTime;
	this->warrior 		= warrior;
}
void Game::printGameTime(void){
	cout<<setw(3)<<setfill('0')<<curTime/60<<":"<<setw(2)<<setfill('0')<<curTime%60<<" ";
}
bool Game::checkGameTime(void){
	//totalTime = 500;			//测试用，限制游戏时间 
	if(curTime >= totalTime)	//到达时间时，退出游戏
		return true;
	else
		return false;
}
void Game::begin(void){
	Warrior *tmp, *tmpRed, *tmpBlue;
	string redName, blueName;
	int ret=0, retRed=0, retBlue=0;
	bool redWin = false;
	bool blueWin = false;
	
	//创建所有城市
	NormalCity city[totalCities];
	for(int i=0;i<totalCities; i++){
		city[i].setCityNum(i+1);
		//cout<<"create normal city, "<<"Number="<<city[i].number<<endl;
	} 

	//创建红蓝基地
	Headquarter *red = new Headquarter(0, "red", initElements, decLoyalty);
	Headquarter *blue = new Headquarter(totalCities+1, "blue", initElements, decLoyalty);
	//red->showCity();
	//blue->showCity();
	
	//游戏正式开始
	while(true){
		//第一阶段，双方基地制造武士
		if(0 == curTime%60){
			redName = red->getWarriorName();		//先获取要制造的武士的名字 
			blueName = blue->getWarriorName();
			//cout<<"redname="<<redName<<",bluename="<<blueName<<endl;
			for(int i=0; i<WARRIOR_KIND; i++){		//按照指定顺序制造武士 
				if(0 == warrior[i].name.compare(redName)){
					red->makeWarrior(warrior[i].element, warrior[i].force, totalCities);
				}
			}
			for(int i=0; i<WARRIOR_KIND; i++){
				if(0 == warrior[i].name.compare(blueName)){
					blue->makeWarrior(warrior[i].element, warrior[i].force, totalCities);
				}	
			} 
			if(checkGameTime())	break;
		}
		
		//第二阶段，5min判断狮子是否逃跑
		curTime += 5;
		if(5 == curTime%60){
			//遍历所有的城市，找出狮子对象 
			for(int i=0; i<totalCities; i++){
				if(city[i].redWarrior){
					tmp = city[i].redWarrior;
					//如果武士是狮子，判断忠诚度，是否会逃跑
					if(0 == tmp->name.compare("lion")){
						Lion *lion = dynamic_cast<Lion *>(tmp);
						if(lion->loyalty <= 0){
							lion->escape();
							red->eraseWarrior(tmp);
							city[i].redWarrior = NULL;
						}	
					} 
				}	
				if(city[i].blueWarrior){
					tmp = city[i].blueWarrior;
					//如果武士是狮子，判断忠诚度，是否会逃跑
					if(0 == tmp->name.compare("lion")){
						Lion *lion = dynamic_cast<Lion *>(tmp);
						if(lion->loyalty <= 0){
							lion->escape();
							blue->eraseWarrior(tmp);
							city[i].blueWarrior = NULL;
						}
					}
				}
			}
			if(checkGameTime())	break; 
		}
		
		//第三阶段，10min武士前进
		curTime += 5;
		if(10 == curTime%60){
			//所以武士完成前进后，再按城市顺序进行汇报
			//如果已经在敌方基地，则无需前进，返回-1
			//如果前进后，到达敌方基地，则返回0，到达常规城市，返回1  
			for(vector<Warrior *>::const_iterator  i=red->warriorVec.begin(); i!=red->warriorVec.end(); ++i){
				retRed = (*i)->march(city);
				if(0 == retRed){
					tmpRed = (*i);	//该武士占领了敌方基地 
					redWin = true;
				}	
			}
			for(vector<Warrior *>::const_iterator  i=blue->warriorVec.begin(); i!=blue->warriorVec.end(); ++i){
				retBlue = (*i)->march(city);
				if(0 == retBlue){
					tmpBlue = (*i);	//该武士占领了敌方基地
					blueWin = true; 	
				}
			}
			//检查红方基地是否被占领 
			if(blueWin){
				//001:10 red iceman 1 reached blue headquarter with 20 elements and force 30
				//003:10 blue headquarter was taken
				printGameTime();
				cout<<"blue "<<tmpBlue->name<<" "<<tmpBlue->number<<" reached red headquarter with "<<tmpBlue->elements<<" elements and force "<<tmpBlue->force<<endl;
				printGameTime();
				cout<<"red headquarter was taken"<<endl;
			}
			//打印普通城市前进信息 
			for(int i=0; i<totalCities; i++){
				if(city[i].redWarrior){
					tmp = city[i].redWarrior;
					Game::printGameTime(); 
					cout<<tmp->team<<" "<<tmp->name<<" "<<tmp->number<<" marched to city "<<city[i].number<<" with "<<tmp->elements<<" elements and force "<<tmp->force<<endl;	
				}
				if(city[i].blueWarrior){
					tmp = city[i].blueWarrior;
					Game::printGameTime(); 
					cout<<tmp->team<<" "<<tmp->name<<" "<<tmp->number<<" marched to city "<<city[i].number<<" with "<<tmp->elements<<" elements and force "<<tmp->force<<endl;
				}
			}
			//检查蓝方基地是否被占领
			if(redWin){
				//001:10 red iceman 1 reached blue headquarter with 20 elements and force 30
				//003:10 blue headquarter was taken
				printGameTime();
				cout<<"red "<<tmpRed->name<<" "<<tmpRed->number<<" reached blue headquarter with "<<tmpRed->elements<<" elements and force "<<tmpRed->force<<endl;
				printGameTime();
				cout<<"blue headquarter was taken"<<endl;
			}
			 
			if(redWin || blueWin)			//有一方到达敌方基地，则游戏结束 
				break;
			if(checkGameTime())	break; 
		}
		
		//第四阶段，35min狼抢夺武器 
		curTime += 25;
		if(35 == curTime%60){
			for(int i=0; i<totalCities; i++){
				//同一城市有两个武士 
				if(city[i].redWarrior && city[i].blueWarrior){
					tmp = city[i].redWarrior; 
					if(0 == tmp->name.compare("wolf")){
						Wolf *wolf = dynamic_cast<Wolf *>(tmp);
						wolf->rob(city[i].blueWarrior);
					}
					tmp = city[i].blueWarrior; 
					if(0 == tmp->name.compare("wolf")){
						Wolf *wolf = dynamic_cast<Wolf *>(tmp);
						wolf->rob(city[i].redWarrior);
					}
				}		
			}
			if(checkGameTime())	break; 
		}

		//第五阶段，40min发生战斗 
		curTime += 5;
		if(40 == curTime%60){
			for(int i=0; i<totalCities; i++){
				tmpRed = city[i].redWarrior;
				tmpBlue = city[i].blueWarrior;

				//同一城市有两个武士则开战，一定要先对武器排序 
				if(tmpRed && tmpBlue){
					ret = city[i].fight(tmpRed, tmpBlue);
					
					if(ret == -2){		//都死了 
						//000:40 both red iceman 1 and blue lion 12 died in city 2
						Game::printGameTime(); 
						cout<<"both red "<<tmpRed->name<<" "<<tmpRed->number<<" and blue "<<tmpBlue->name<<" "<<tmpBlue->number<<" died in city "<<city[i].number<<endl;
					}
					else if(ret == -1){	//蓝方活着 
						//008:40 red iceman 6 killed blue ninja 3 in city 4 remaining 85 elements
						Game::printGameTime(); 
						cout<<"blue "<<tmpBlue->name<<" "<<tmpBlue->number<<" killed red "<<tmpRed->name<<" "<<tmpRed->number<<" in city "<<city[i].number<<" remaining "<<tmpBlue->elements<<" elements"<<endl;
						tmpBlue->getEnemyWeapon(tmpRed);
					}
					else if(ret == 1){	//红方活着 
						Game::printGameTime(); 
						cout<<"red "<<tmpRed->name<<" "<<tmpRed->number<<" killed blue "<<tmpBlue->name<<" "<<tmpBlue->number<<" in city "<<city[i].number<<" remaining "<<tmpRed->elements<<" elements"<<endl;
						tmpRed->getEnemyWeapon(tmpBlue);
					}
					else if(ret == 0){	//都活着 
						//008:40 both red lion 7 and blue dragon 2 were alive in city 3
						Game::printGameTime(); 
						cout<<"both red "<<tmpRed->name<<" "<<tmpRed->number<<" and blue "<<tmpBlue->name<<" "<<tmpBlue->number<<" were alive in city "<<city[i].number<<endl;
					}
					//活着的龙，欢呼 
					if(tmpRed->alive() && (0 == tmpRed->name.compare("dragon"))){
						Dragon *dragon = dynamic_cast<Dragon *>(tmpRed);
						dragon->yelled(); 
					}
					if(tmpBlue->alive() && (0 == tmpBlue->name.compare("dragon"))){
						Dragon *dragon = dynamic_cast<Dragon *>(tmpBlue);
						dragon->yelled(); 
					}
					//删除死去的武士对象
					if(!tmpRed->alive()) {
						red->eraseWarrior(tmpRed);
						city[i].redWarrior = NULL;
					}	
					if(!tmpBlue->alive()){
						blue->eraseWarrior(tmpBlue);
						city[i].blueWarrior = NULL; 
					}
				}
			}
			if(checkGameTime())	break; 
		}
		//第六阶段，50min司令部报告剩余生命元 
		curTime += 10;
		if(50 == curTime%60){
			red->report();
			blue->report();
			if(checkGameTime())	break; 
		}
		
		//第七阶段，55min武士报告拥有武器情况 
		curTime += 5;
		if(55 == curTime%60){
			for(int i=0; i<totalCities; i++){
				tmpRed = city[i].redWarrior;
				tmpBlue = city[i].blueWarrior;
				if(tmpRed)	tmpRed->showInfo();
				if(tmpBlue)	tmpBlue->showInfo();
			}
			if(checkGameTime())	break; 
		}
		
		//第八阶段，60min 
		curTime += 5;
	} 
}


#if DEBUG
int main(){
	// case 1
	gameInfo_t gInfo = {
		.testCase = 1,
		.initElements = 6000,
		.totalCities = 10,
		.decLoyalty = 10,
		.totalTime = 4000,
		.warrior ={
			{"dragon",	120,	50},
			{"ninja",	20,		50},
			{"iceman",	130,	50},
			{"lion",	10,		50},
			{"wolf",	20,		150}
		}
	};
	cout<<"Case "<<gInfo.testCase<<":"<<endl;
	Game game(gInfo.initElements, gInfo.totalCities, gInfo.decLoyalty, gInfo.totalTime, gInfo.warrior);
	game.begin();	
}

#endif
