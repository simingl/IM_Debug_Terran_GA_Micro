#include <AttackManager.h>

using namespace BWAPI;

AttackManager::AttackManager(Arbitrator::Arbitrator<BWAPI::Unit*,double>* arbitrator)
{
	this->arbitrator = arbitrator;
	lastFrameCheck = 0;
	lastExpandFrame = 0;
	lastAttack = 0;
    initialized = false;

}

AttackManager::~AttackManager()
{

}

void AttackManager::setDefenseManager(DefenseManager* defenseManager)
{
	this->defenseManager = defenseManager;
}

void AttackManager::setInformationManager(InformationManager* infoManager)
{
	this->informationManager = infoManager;
}

void AttackManager::setBuildOrderManager(BuildOrderManager* buildOrderManager)
{
	this->buildOrderManager = buildOrderManager;
}


void AttackManager::setBaseManager(BaseManager *baseManager)
{
	this->baseManager = baseManager;
}

int AttackManager::getStartAttack()
{
	return startAttack;
}

bool AttackManager::getFirstAttackDone()
{
	return firstAttackDone;
}

void AttackManager::setFirstAttackDone(bool firstAttackDone)
{
	this->firstAttackDone = firstAttackDone;
}


double AttackManager::enemyDPS()
{
	double total = 0;

	for each (BWAPI::Unit *u in SelectAllEnemy()(canMove)(isCompleted).not(isWorker))
	{
		total += ((double)u->getType().groundWeapon().damageAmount())/u->getType().groundWeapon().damageCooldown();
	}

	return total;
}

double AttackManager::myDPS()
{
	double total = 0;

	for each (BWAPI::Unit *u in SelectAll()(canMove)(isCompleted).not(isWorker))
	{
		total += ((double)u->getType().groundWeapon().damageAmount())/u->getType().groundWeapon().damageCooldown();
	}

	return total;
}

double AttackManager::myDPS(std::map<BWAPI::Unit*, AttackData> group)
{
	double total = 0;
	std::set<BWAPI::Unit*> groupSet;

	for (std::map<BWAPI::Unit*, AttackData>::const_iterator it = group.begin(); it != group.end(); it++)
	{
		groupSet.insert(it->first);
	}

	for each (BWAPI::Unit *u in groupSet)
	{
		if (u->exists())
			total += ((double)u->getType().groundWeapon().damageAmount())/u->getType().groundWeapon().damageCooldown();
	}

	return total;
}

int AttackManager::enemyHP()
{
	int total = 0;

	for each (BWAPI::Unit *u in SelectAllEnemy()(canMove)(isCompleted).not(isWorker))
	{
		if (u->exists())
		{
			total += u->getHitPoints();			
			total += u->getShields();
		}
	}

	return total;
}

int AttackManager::myHP(std::map<BWAPI::Unit*, AttackData> group)
{
	int total = 0;
	std::set<BWAPI::Unit*> groupSet;

	for (std::map<BWAPI::Unit*, AttackData>::const_iterator it = group.begin(); it != group.end(); it++)
	{
		groupSet.insert(it->first);
	}

	for each (BWAPI::Unit *u in groupSet)
	{
		if (u->exists())
		{
			total += u->getHitPoints();
			total += u->getShields();
		}
	}

	return total;
}


void AttackManager::update()
{

	if (Broodwar->getFrameCount() > lastFrameCheck + 48 )
	{
		lastFrameCheck = Broodwar->getFrameCount();
	
	}
}


std::string AttackManager::getName() const
{
	return "Attack Manager";
}

std::string AttackManager::getShortName() const
{
	return "Att";
}

void AttackManager::onOffer(std::set<BWAPI::Unit*> units)
{
	for each (BWAPI::Unit *u in units)
	{
		if (company.find(u) == company.end())
		{
			arbitrator->accept(this, u);
			AttackManager::AttackData	ad;
			ad.mode = AttackManager::AttackData::Idle;
			ad.target = NULL;
			company[u] = ad;
		}
	}
}

void AttackManager::onRevoke(BWAPI::Unit *u, double bid)
{
	onRemoveUnit(u);
}

void AttackManager::onRemoveUnit(BWAPI::Unit* u)
{
	company.erase(u);
}