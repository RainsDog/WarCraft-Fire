#include <iostream>
#include "game.h"


using namespace std;
	
int main(void){
	gameInfo_t gInfo;
	cin>>gInfo.testCase;
	cin>>gInfo.initElements>>gInfo.totalCities>>gInfo.decLoyalty>>gInfo.totalTime;
	cin>>gInfo.warrior[0].element>>gInfo.warrior[1].element>>gInfo.warrior[2].element>>gInfo.warrior[3].element>>gInfo.warrior[4].element;
	cin>>gInfo.warrior[0].force>>gInfo.warrior[1].force>>gInfo.warrior[2].force>>gInfo.warrior[3].force>>gInfo.warrior[4].force;
	gInfo.warrior[0].name = "dragon";
	gInfo.warrior[1].name = "ninja";
	gInfo.warrior[2].name = "iceman";
	gInfo.warrior[3].name = "lion";
	gInfo.warrior[4].name = "wolf";

	cout<<"Case "<<gInfo.testCase<<":"<<endl;
	Game game(gInfo.initElements, gInfo.totalCities, gInfo.decLoyalty, gInfo.totalTime, gInfo.warrior);
	game.begin();
	return 0;
}
