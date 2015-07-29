#pragma once
#include <algorithm>
#include <Arbitrator.h>
#include <DefenseManager.h>
#include <InformationManager.h>
#include <UnitGroupManager.h>
#include <BaseManager.h>
#include <BuildOrderManager.h>
#include <BorderManager.h>
#include "Squad.h"
#include <BWAPI.h>
#include <BWTA.h>


class StrategyManager
{
public:
	StrategyManager();
	~StrategyManager();
	void setDefenseManager		(DefenseManager*);
	void setInformationManager	(InformationManager*);
	void setBaseManager			(BaseManager*);
	void setBuildOrderManager	(BuildOrderManager*);
	void setBorderManager	    (BorderManager*);

	void		update			();

	class Points
	{
		public:
			int firebat;
			int marine;
			int ghost;
			int goliath;
			int medic;
			int tank;
			int vulture;

			int battlecruiser;
			int sciencevessel;
			int dropship;
			int nuclearmissile;
			int valkyrie;
			int wraith;
	};
private:
	DefenseManager								*defenseManager;
	InformationManager							*informationManager;
	BaseManager									*baseManager;
	BuildOrderManager                           *buildOrderManager;
	BorderManager                               *borderManager;

	Points                                      *unitPoints;
	int enemyBaseNumber;
	
	int lastFrameCheck;
};