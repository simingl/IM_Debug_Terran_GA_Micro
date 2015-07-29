#pragma once

#include <algorithm>
#include <math.h>
#include <UnitGroupManager.h>
#include <BWAPI.h>
#include <BWTA.h>
#include <InformationManager.h>
#include <IMEnemy.h>
#include <IMSelfSum.h>
#include "vmmlib/vector.hpp"
#include "IMParams.h"
#include "GA.h"

class Squad
{
public:
	enum SquadMode
	{
		Idle,
		Moving,
		Attacking,
		Fleeing
	};

	Squad();

	void setInformationManager	(InformationManager*);

	const BWAPI::Position getTargetPosition() const;
	void setTargetPosition(BWAPI::Position position);
	BWAPI::Position* getSquadPosition();
	void attackPosition(BWAPI::Position target);
	void holdPosition(BWAPI::Position target);
	void addMarine(BWAPI::Unit* u);
	void addGoliath(BWAPI::Unit* u);
	void addTanks(BWAPI::Unit* u);
	void addVulture(BWAPI::Unit* u);
	void removeMember(BWAPI::Unit* u);
	void updateCurrentPosition();
	int  getSquadUnitCount();
	int  getTanksCount();
	int  getGoliathCount();
	int  getVultureCount();
	int  getSquadScore();
	bool isSquadMoving();
	bool isSquadMember(BWAPI::Unit* u);
	void setSquadID(int squadId);
	void draw();
    int getSquadHP();
	void updateSquadFormation();
	SquadMode getSquadMode();
    void battleFormation(BWAPI::Position defencePoint, BWAPI::Position enemyPoint);
    void battleAttackFormation(BWAPI::Position defencePoint, BWAPI::Position enemyPoint);
	bool isSquadInPosition();
	bool battleAttackFormation();
	void potentialMove(BWAPI::Unit* unit, BWAPI::Position* target, bool withBlock=true);

private:
	SquadMode mode;
    double totalDPS;
	double    totalHP;
	int id;
	BWAPI::Position target;

	InformationManager *informationManager;
	IMEnemy  *eIM;
	
	
	IMSelfSum   *sIMsum;

	std::set<BWAPI::Unit*> marines;
	std::set<BWAPI::Unit*> medics;
	std::set<BWAPI::Unit*> tanks;
	std::set<BWAPI::Unit*> vultures;
	std::set<BWAPI::Unit*> goliaths;
	std::set<BWAPI::Unit*> allUnits;

	BWAPI::Position currentPosition;
	void updateSquadDPS_HP();
	int getDistance(BWAPI::Position* p1, BWAPI::Position* p2);
	BWAPI::Position getTempFormationPosition(BWAPI::Position* p1, BWAPI::Position* p2, int distance);
	BWAPI::Unit* getTargetInSightRange(BWAPI::Unit* unit);
	
	//potential args
	double U, A, B, C, n, m, l, d, repulsionThreshold;

	IMParams params;
};

