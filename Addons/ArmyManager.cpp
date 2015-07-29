#include <ArmyManager.h>

using namespace BWAPI;

ArmyManager::ArmyManager(Arbitrator::Arbitrator<BWAPI::Unit*,double>* arbitrator)
{
	params = GA::getInstance()->getParams();

	this->recordFilename = new string("c:\\lsm\\record.data");
	this->count = 0;

	this->squadmanager = new SquadManager();

	this->arbitrator = arbitrator;
	lastAntiClockFrame = 0;
	lastAttackFrameCheck = 0;
	lastExpandFrame = 0;
	lastAttack = 0;
    initialized = false;
	lastAttackedFrame = 0;
    latestAttackedPosition = BWAPI::Broodwar->getMousePosition();
	startAttack = false;

	squad.setSquadID(0);
}

ArmyManager::~ArmyManager()
{
  delete this->recordFilename;
  delete this->squadmanager;
}

void ArmyManager::antiCloak(BWAPI::Position position)
{
	UnitGroup comsats = SelectAll(UnitTypes::Terran_Comsat_Station)(isCompleted);
	for each(BWAPI::Unit *c in comsats)
	{
		if(c->getEnergy() > 50)
		{
			c->useTech(TechTypes::Scanner_Sweep, position);
			this->lastAntiClockFrame = Broodwar->getFrameCount();
			break;
		}
	}
}

void ArmyManager::setInformationManager(InformationManager* infoManager)
{
	this->informationManager = infoManager;
	this->enemyAIM = this->informationManager->getIMEnemy();
	this->squad.setInformationManager(informationManager);
}

int ArmyManager::getStartAttack()
{
	return startAttack;
}

bool ArmyManager::isInitialized()
{
	return initialized;
}

void ArmyManager::initialize()
{
	this->initialized = true;
}

double ArmyManager::enemyDPS()
{
	double total = 0;

	for each (BWAPI::Unit *u in SelectAllEnemy()(canMove)(isCompleted).not(isWorker))
	{
		total += ((double)u->getType().groundWeapon().damageAmount())/u->getType().groundWeapon().damageCooldown();
	}

	return total;
}

int ArmyManager::enemyHP()
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

int ArmyManager::myHP()
{
	return this->squad.getSquadHP();
}

void ArmyManager::update()
{
	this->squadmanager->onFrame();
	return;

	// Bid on all completed military units
	std::set<BWAPI::Unit*> myPlayerUnits=BWAPI::Broodwar->self()->getUnits();
	for (std::set<BWAPI::Unit*>::iterator u = myPlayerUnits.begin(); u != myPlayerUnits.end(); u++)
	{
		if ((*u)->isCompleted() && 
			!(*u)->getType().isWorker() && 
			!(*u)->getType().isBuilding() &&
			(*u)->getType() != BWAPI::UnitTypes::Zerg_Egg &&
			(*u)->getType() != BWAPI::UnitTypes::Zerg_Larva)
		{
			arbitrator->setBid(this, *u, 50);
		}
	}

    int current = Broodwar->getFrameCount();

	if ( current > (lastAttackFrameCheck + 3) )
	{
		lastAttackFrameCheck = current;
		if(squad.isSquadInPosition())
			startAttack = true;
		
		/*if(startAttack)
		{
			Position ptank = this->informationManager->getIMEnemyTank()->getTanksCenter();
			Position pmarine = this->informationManager->getIMEnemyMarine()->getMarineCenter();
			if(ptank.x() > 100 && ptank.y() >100)
			{
				this->attackPosition(ptank);
			}
			else if(pmarine.x() > 100 && pmarine.y() >100)
			{
				this->attackPosition(pmarine);
			}
		}
		else
		{
			this->battlePrepareFormation();
		}*/
	}

	squad.draw();
}



void ArmyManager::battleFormation(BWAPI::Position defencePoint, BWAPI::Position enemyStrong)
{
	squad.battleAttackFormation(defencePoint, enemyStrong);
}

bool ArmyManager::battlePrepareFormation()
{
	return squad.battleAttackFormation();
}

void ArmyManager::attackPosition(BWAPI::Position targetPosition)
{
	squad.attackPosition(targetPosition);
}

std::string ArmyManager::getName() const
{
	return "Army Manager";
}

std::string ArmyManager::getShortName() const
{
	return "Arm";
}

int ArmyManager::getScore()
{
	int score = this->squadmanager->getSquadScore();
	//int score = this->squadmanager->getSquadDestroyScore(); 
	if(true||score >=500){
		this->record2file(score);
	}
	return score;
}

void ArmyManager::onOffer(std::set<BWAPI::Unit*> units)
{
	//for each (BWAPI::Unit *u in units)
	//{
		//if (company.find(u) == company.end())
		//{
		//	int team = 1;
		//	arbitrator->accept(this, u);
		//	ArmyManager::ArmyData ad(u);
		//	ad.mode = ArmyManager::ArmyData::Idle;
		//	ad.target = NULL;
		//}

		//if ((*u).getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode ||(*u).getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Siege_Mode)
		//{
		//	squad.addTanks(u);
		//}
		//else if ((*u).getType() == BWAPI::UnitTypes::Terran_Goliath)
		//{
		//	squad.addGoliath(u);
		//}
		//else if ((*u).getType() == BWAPI::UnitTypes::Terran_Vulture)
		//{
		//	squad.addVulture(u);
		//}
		//else if ((*u).getType() == BWAPI::UnitTypes::Terran_Marine)
		//{
		//	squad.addMarine(u);
		//}

	//}
}

void ArmyManager::onRevoke(BWAPI::Unit *u, double bid)
{
	onRemoveUnit(u);
}

void ArmyManager::onRemoveUnit(BWAPI::Unit* u)
{
	this->squadmanager->onUnitDestroy(u);
	//company.erase(u);

	//if (u->getType()==BWAPI::UnitTypes::Terran_Siege_Tank_Siege_Mode ||
	//	u->getType()==BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode ||
	//	u->getType()==BWAPI::UnitTypes::Terran_Goliath||
	//	u->getType()==BWAPI::UnitTypes::Terran_Vulture ||
	//	u->getType()==BWAPI::UnitTypes::Terran_Marine)
	//{
	//	if(squad.isSquadMember(u))
	//	{
	//		squad.removeMember(u);
	//	}
	//}
}

bool ArmyManager::isAttackedCheck()
{
	std::set<BWAPI::Unit*> myPlayerUnits=BWAPI::Broodwar->self()->getUnits();
	for (std::set<BWAPI::Unit*>::iterator u = myPlayerUnits.begin(); u != myPlayerUnits.end(); u++)
	{
		if ((*u)->isUnderAttack())
		{
			this->latestAttackedPosition = (*u)->getPosition();
			this->lastAttackedFrame = Broodwar->getFrameCount();
			return true;
		}
	}
	return false;
}

void ArmyManager::onUnitDestroy(BWAPI::Unit* unit){
	this->squadmanager->onUnitDestroy(unit);
}

void ArmyManager::record2file(int score)
{
	int eu = this->squadmanager->getEnemies().size();
	int fu = this->squadmanager->getSquadUnits().size();

	ofstream ofile;
	ofile.open(this->recordFilename->c_str(), ios::app);
	ofile <<   "Score: "<< score << ". " ; 

	ofile<< " GA: "<<params.UnitValue<<","<<params.UnitRadius<<","<<params.TerrainValue<<","<<params.TerrainRadius<<"; "
		<<params.A<<","<<params.B<<","<<params.m<<params.n<<"; "
		<<params.m_freeze<<","<<params.m_kitingRange<<","<<params.m_targetNearby<<","
		<<params.m_kitingDist<<","<<params.m_hpfocus<<","<<params.m_hpkiting<< ". "
		<<"EnemyUnitsLeft:"<<eu<<", Friendly UnitsLeft: "<<fu;

	ofile<<endl;

	ofile.close();
}