#pragma once
#include <Arbitrator.h>
#include <DefenseManager.h>
#include <InformationManager.h>
#include <UnitGroupManager.h>
#include <BaseManager.h>
#include <BuildOrderManager.h>
#include <BWAPI.h>
#include <BWTA.h>


class AttackManager : Arbitrator::Controller<BWAPI::Unit*,double>
{
public:
	class AttackData
	{
	public:
		enum AttackMode
		{
			Idle,
			Moving,
			Attacking,
			Fleeing
		};
		AttackData(){ mode = Idle; lastFrameDragoonAttack = 0; }
		BWTA::BaseLocation* target;
		AttackMode mode;
		int lastFrameDragoonAttack;
	};

	AttackManager(Arbitrator::Arbitrator<BWAPI::Unit*,double>* arbitrator);
	~AttackManager();
	void setDefenseManager		(DefenseManager*);
	void setInformationManager	(InformationManager*);
	void setBaseManager			(BaseManager*);
	void setBuildOrderManager	(BuildOrderManager*);
	//void setMoodManager			(MoodManager*);

	void setLastExpandFrame		(int);
	//void setMoodPlan			(MoodManager::MoodData::Mood);

	virtual void		onOffer			(std::set<BWAPI::Unit*> unitGroup);
	virtual void		onRevoke		(BWAPI::Unit *p, double bid);
	void				onRemoveUnit	(BWAPI::Unit *unit);
	virtual void		update			();
	virtual	std::string getName			() const;
	virtual	std::string getShortName	() const;

	double							enemyDPS		();
	double							myDPS			();
	double							myDPS			(std::map<BWAPI::Unit*, AttackData>);
	int								enemyHP			();
	int								myHP			(std::map<BWAPI::Unit*, AttackData>);
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
	DefenseManager								*defenseManager;
	InformationManager							*informationManager;
	BaseManager									*baseManager;
	BuildOrderManager                           *buildOrderManager;

	//Random	*randomEarlyAttack;
	//Random	*randomSizeRush;

	std::map<BWAPI::Unit*, AttackData>	company;
	std::set<BWAPI::Unit*>				fleeing;
	std::vector<BWAPI::Position>		enemyBuildings;
	std::vector<BWTA::BaseLocation*>	unvisitedBases;

	double	sizeRush;
	int		lastFrameCheck;
	int		lastExpandFrame;
	int		lastAttack;
	bool	firstAttackDone;
	bool	initialized;
	int		startAttack;
	int		round;
	int		roundUnvisited;
};