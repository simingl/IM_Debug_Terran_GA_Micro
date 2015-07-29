#pragma once

#include "MicroAspect.h"
#include "GA.h"
#include "vmmlib/vector.hpp"
#include "InformationManager.h"
#include "IMParams.h"

using namespace BWAPI;
using namespace std;



class SquadManager
{
public:
	enum SquadState
	{
		Idle,
		Approaching,
		Attacking,
		Fleeing
	};
	SquadManager();
	void loadUnits();
	void addUnit(Unit* unit);
	void addEnemy(Unit* unit);
	void onUnitDestroy(Unit* unit);

    void onFrame();  //check every frame
	void onFire();   //in a combat
	
	void dodge();    //select the fewer HPs and dodge
	bool isSquadInCombat();
	bool isSquadMoving();
	void switchState(SquadState ns){
		state = ns;
	}
	std::set<Unit*> getSquadUnits(){
		return unitSet;
	}
	std::set<Unit*> getEnemies(){
		return enemySet;
	}

	void potentialMove(Unit* unit, Position* target, int dist);

	SquadState state;
	InformationManager *im;

	~SquadManager(){}; 
	BWAPI::Unit* getTarget(){return this->target;};
	int getSquadScore();
	int getSquadDestroyScore();
private:
	void regroup();
	void refreshGroupPosition();
	BWAPI::Unit* setTarget();
	

	std::set<BWAPI::Unit*> unitSet;
	std::set<MicroAspect*> microAspectSet;
	std::map<Unit*, MicroAspect*> unitToMicroAspectMap;

	std::set<Unit*> enemySet;
	
	void draw();
	
	bool needsregroup;

	BWAPI::Unit* target;

	int centerX;
	int centerY;
	int radius;
	int threashold;

	IMParams params;
	//potential args
	double U, A, B, C, n, m, l, d, repulsionThreshold, repulsionThresholdEnemy;

	
};


