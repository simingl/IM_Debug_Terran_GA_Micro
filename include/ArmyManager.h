#pragma once
#include <algorithm>
#include <Arbitrator.h>
#include <InformationManager.h>
#include <UnitGroupManager.h>
#include <Squad.h>
#include <BWAPI.h>
#include <BWTA.h>
#include <IMEnemy.h>
#include <IMSelfSum.h>
#include <UnitGroup.h>
#include <fstream>
#include "SquadManager.h"
#include "IMParams.h"

class ArmyManager : Arbitrator::Controller<BWAPI::Unit*,double>
{
public:
	class ArmyData
	{
	public:
		enum ArmyMode
		{
			Idle,
			Moving,
			Attacking,
			Fleeing
		};
		ArmyData(BWAPI::Unit* u){ unit = u; mode = Idle; lastFrameDragoonAttack = 0; }
		BWAPI::Unit* unit;
		BWTA::BaseLocation* target;
		ArmyMode mode;
		int lastFrameDragoonAttack;
	};

	class TeamData
	{
	public:
		enum TeamMode
		{
			Idle,
			Moving,
			Attacking,
			Fleeing
		};
		TeamData(){mode = Idle;}
		std::set<BWAPI::Unit*> marines;
		std::set<BWAPI::Unit*> medics;
		std::set<BWAPI::Unit*> tanks;
		BWAPI::Unit* scienceVessel;

		BWAPI::Position* target;
		TeamMode mode;
	};
	ArmyManager(Arbitrator::Arbitrator<BWAPI::Unit*,double>* arbitrator);
	~ArmyManager();

	void setInformationManager	(InformationManager*);
	InformationManager* getInformationManager(){return this->informationManager;};

	void setLastExpandFrame		(int);

	virtual void		onOffer			(std::set<BWAPI::Unit*> unitGroup);
	virtual void		onRevoke		(BWAPI::Unit *p, double bid);
	void				onRemoveUnit	(BWAPI::Unit *unit);
	virtual void		update			();
	virtual	std::string getName			() const;
	virtual	std::string getShortName	() const;
	
	void onUnitDestroy(BWAPI::Unit* unit);

	double							enemyDPS		();
	int								enemyHP			();
	int								myHP			();
	int                             getScore   ();
	static std::set<BWAPI::Unit*>	whoIsAttacking	(BWAPI::Unit*);
	static bool						containsBuilding(std::set<BWAPI::Unit*>);
	static BWAPI::Unit*				nearestUnit		(BWAPI::Unit*, std::set<BWAPI::Unit*>);

	bool	isInitialized		();
	void	initialize			();
	int		getStartAttack		();
	bool	getFirstAttackDone	();
	void	setFirstAttackDone	(bool);

private:
	Arbitrator::Arbitrator<BWAPI::Unit*,double> *arbitrator;
	InformationManager							*informationManager;
	IMEnemy                           *enemyAIM;

	std::map<BWAPI::Unit*, ArmyData>	company;
	std::set<BWAPI::Unit*>				fleeing;
	std::vector<BWAPI::Position>		enemyBuildings;
	std::vector<BWTA::BaseLocation*>	unvisitedBases;

	std::set<BWAPI::Position>        targetsSet;

	Squad              squad;

	bool isTeamMoving(const TeamData& team);
	bool isTeamEmpty(const TeamData& team);
	void battleFormation(BWAPI::Position defencePoint, BWAPI::Position enemyStrong);
	bool battlePrepareFormation();
	void attackPosition(BWAPI::Position targetPosition);
	bool isAttackedCheck();
	BWAPI::Unit* getNextAttackTarget();
	BWAPI::Position getNextEnemyGroup();

	std::set<BWAPI::Unit*> getAttacker(BWAPI::Unit *unit);
	void updateMicroInCombat();

	double	sizeRush;
	int		lastAntiClockFrame;
	int		lastAttackFrameCheck;
	int		lastExpandFrame;
	int		lastAttack;
	int  	lastAttackedFrame;
	bool	initialized;
	bool	startAttack;
	int		round;
	int		roundUnvisited;

	BWAPI::Position latestAttackedPosition;
	std::set<BWTA::Chokepoint*> myBorder;
	std::vector<BWTA::Chokepoint*> myBorderVector;
	void antiCloak(BWAPI::Position position);

	string* recordFilename;
	void record2file(int score);

	int count;
	IMParams params;
	SquadManager *squadmanager;
};