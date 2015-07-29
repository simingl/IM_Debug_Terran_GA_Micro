#pragma once

#include "microparams.h"
#include <BWAPI.h>
#include "IMParams.h"

class SquadManager;

class MicroAspect
{
public:
	MicroAspect(BWAPI::Unit* u, SquadManager* s);

	BWAPI::Unit* getTarget(std::set<BWAPI::Unit*> &enemies);
	

	void onFire(std::set<BWAPI::Unit*> &enemies);
	void dodge(std::set<BWAPI::Unit*> &enemies);
	bool isBeingTarget(BWAPI::Unit* u, std::set<BWAPI::Unit*> &enemies);
	bool isUnitMoving();
	void kiteMove(std::set<BWAPI::Unit*> &enemies);
    ~MicroAspect(); 

private:
	BWAPI::Unit* getEnemyAttackingThisUnit(std::set<BWAPI::Unit*> &enemies);

	BWAPI::Unit * unit;
	SquadManager * squad;
	
	BWAPI::Unit * target;
	void draw();
	BWAPI::Position getKitingPosition(BWAPI::Unit* u, BWAPI::Unit* enemy);
	BWAPI::Position getKitingPositionByIM(BWAPI::Position* pos);
	int getNumberOfAttackers(BWAPI::Unit* u);
	
	MicroParam microparam;
	IMParams params;
	IMParams params_range;

	std::pair<int, BWAPI::Position> position;
	void updatePosition();

	float getUnitHPPercent(BWAPI::Unit* u);
	BWAPI::Unit* getNearestUnit(std::set<BWAPI::Unit*> &enemies);
	BWAPI::Unit* getLowHPUnit(std::set<BWAPI::Unit*> &nearbyunits);
	BWAPI::Unit* getHighFocusUnit(std::set<BWAPI::Unit*> &nearbyunits);

	void switchParams(bool melee);	
};




