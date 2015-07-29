#include <StrategyManager.h>

using namespace BWAPI;

StrategyManager::StrategyManager()
{
	int firebat = 0;
	int marine = 100;
	int ghost = 0;
	int goliath = 0;
	int medic = 0;
	int tank = 0;
	int vulture = 0;

	int battlecruiser = 0;
	int dropship = 0;
	int nuclearmissile = 0;
	int valkyrie = 0;
	int wraith = 0;
	int sciencevessel = 0;

	int lastFrameCheck=0;
}

StrategyManager::~StrategyManager()
{

}

void StrategyManager::setDefenseManager(DefenseManager* defenseManager)
{
	this->defenseManager = defenseManager;
}

void StrategyManager::setInformationManager(InformationManager* infoManager)
{
	this->informationManager = infoManager;
}

void StrategyManager::setBuildOrderManager(BuildOrderManager* buildOrderManager)
{
	this->buildOrderManager = buildOrderManager;
}


void StrategyManager::setBaseManager(BaseManager *baseManager)
{
	this->baseManager = baseManager;
}

void StrategyManager::setBorderManager(BorderManager* borderManager)
{
	this->borderManager = borderManager;
}

void StrategyManager::update()
{
	if ((lastFrameCheck == 0) || (BWAPI::Broodwar->getFrameCount() > lastFrameCheck + 72))
	{
		lastFrameCheck = BWAPI::Broodwar->getFrameCount();

		if ((BWAPI::Broodwar->getFrameCount() > 5200) && (unitPoints->firebat == 0))
			unitPoints->firebat = 20;

		if ((BWAPI::Broodwar->getFrameCount() > 5200) && (unitPoints->medic == 0))
			unitPoints->medic = 50;

		if ((BWAPI::Broodwar->getFrameCount() > 8500) && (unitPoints->tank == 0) && (BWAPI::Broodwar->enemy()->getRace() == BWAPI::Races::Zerg))
			unitPoints->tank = 50;

		if ((BWAPI::Broodwar->getFrameCount() > 10000) && (unitPoints->sciencevessel == 0))
		{
			if (BWAPI::Broodwar->enemy()->getRace() == BWAPI::Races::Zerg)
				unitPoints->sciencevessel = 20;
			else	
				unitPoints->sciencevessel = 50;
		}

		if ((BWAPI::Broodwar->getFrameCount() > 11000) && (unitPoints->goliath == 0))
			unitPoints->goliath = 50;

		int minerals	= BWAPI::Broodwar->self()->minerals();
		int gas			= BWAPI::Broodwar->self()->gas();

		// If we have too much minerals regarding gas, priority to marines
		if (minerals > gas + 300)
		{
			unitPoints->marine = 200;
			if (unitPoints->tank != 50)
			{
				unitPoints->tank = 50;
			}
		}

		// If we have too much gas regarding minerals, priority to dragoons
		if (gas > minerals + 300 )
		{
			unitPoints->tank = 100;
		}

		//// else, restore
		//if (minerals <= gas + 300 && gas <= minerals + 300 && restore)
		//{
		//	unitPoints->zealot	= unitPoints->zealotLastValue;
		//	unitPoints->dragoon = unitPoints->dragoonLastValue;
		//	restore = false;
		//}

		//if (unitPoints->corsair < 30 &&
		//	((informationManager->enemyHasBuilt(BWAPI::UnitTypes::Terran_Starport))		||
		//	(informationManager->enemyHasBuilt(BWAPI::UnitTypes::Zerg_Spire))			||
		//	(informationManager->enemyHasBuilt(BWAPI::UnitTypes::Zerg_Greater_Spire))	||
		//	(informationManager->enemyHasBuilt(BWAPI::UnitTypes::Protoss_Stargate))))
		//{
		//	unitPoints->corsairLastValue	= unitPoints->corsair;
		//	unitPoints->corsair				= 30;
		//}

		//if (unitPoints->corsair < 60 &&
		//	((informationManager->enemyHasBuilt(BWAPI::UnitTypes::Terran_Wraith))			||
		//	(informationManager->enemyHasBuilt(BWAPI::UnitTypes::Terran_Valkyrie))			||
		//	(informationManager->enemyHasBuilt(BWAPI::UnitTypes::Terran_Battlecruiser))	||

		//	(informationManager->enemyHasBuilt(BWAPI::UnitTypes::Protoss_Corsair))			||
		//	(informationManager->enemyHasBuilt(BWAPI::UnitTypes::Protoss_Scout))			||
		//	(informationManager->enemyHasBuilt(BWAPI::UnitTypes::Protoss_Carrier))			||

		//	(informationManager->enemyHasBuilt(BWAPI::UnitTypes::Zerg_Guardian))			||
		//	(informationManager->enemyHasBuilt(BWAPI::UnitTypes::Zerg_Queen))))
		//{
		//	unitPoints->corsairLastValue	= unitPoints->corsair;
		//	unitPoints->corsair				= 60;
		//}

		//if (unitPoints->corsair < 70 &&
		//	((informationManager->enemyHasBuilt(BWAPI::UnitTypes::Zerg_Mutalisk)) ||
		//	(informationManager->enemyHasBuilt(BWAPI::UnitTypes::Zerg_Devourer))))
		//{
		//	unitPoints->corsairLastValue	= unitPoints->corsair;
		//	unitPoints->corsair				= 70;
		//}

		//if ((informationManager->enemyHasBuilt(BWAPI::UnitTypes::Terran_Wraith))			||
		//	(informationManager->enemyHasBuilt(BWAPI::UnitTypes::Terran_Battlecruiser))	||

		//	(informationManager->enemyHasBuilt(BWAPI::UnitTypes::Protoss_Scout))		||
		//	(informationManager->enemyHasBuilt(BWAPI::UnitTypes::Protoss_Carrier))		||

		//	(informationManager->enemyHasBuilt(BWAPI::UnitTypes::Zerg_Mutalisk))		||
		//	(informationManager->enemyHasBuilt(BWAPI::UnitTypes::Zerg_Devourer)))
		//{
		//	int nbWraith	=	SelectAllEnemy(BWAPI::UnitTypes::Terran_Wraith)(exists).size() + 
		//		informationManager->getNumberEvadedUnits(BWAPI::UnitTypes::Terran_Wraith);

		//	int nbBC		=	SelectAllEnemy(BWAPI::UnitTypes::Terran_Battlecruiser)(exists).size() + 
		//		informationManager->getNumberEvadedUnits(BWAPI::UnitTypes::Terran_Battlecruiser);

		//	int nbScout		=	SelectAllEnemy(BWAPI::UnitTypes::Protoss_Scout)(exists).size() + 
		//		informationManager->getNumberEvadedUnits(BWAPI::UnitTypes::Protoss_Scout);

		//	int nbCarrier	=	SelectAllEnemy(BWAPI::UnitTypes::Protoss_Carrier)(exists).size() + 
		//		informationManager->getNumberEvadedUnits(BWAPI::UnitTypes::Protoss_Carrier);

		//	int nbMuta		=	SelectAllEnemy(BWAPI::UnitTypes::Zerg_Mutalisk)(exists).size() + 
		//		informationManager->getNumberEvadedUnits(BWAPI::UnitTypes::Zerg_Mutalisk);

		//	int nbDevo		=	SelectAllEnemy(BWAPI::UnitTypes::Zerg_Devourer)(exists).size() + 
		//		informationManager->getNumberEvadedUnits(BWAPI::UnitTypes::Zerg_Devourer);

		//	int myCorsairs = SelectAll(BWAPI::UnitTypes::Protoss_Corsair)(exists).size();

		//	if (BWAPI::Broodwar->enemy()->getRace() == BWAPI::Races::Terran)
		//	{
		//		if (myCorsairs < nbWraith + nbBC)
		//		{
		//			unitPoints->corsairLastValue		= unitPoints->corsair;
		//			unitPoints->corsair					= 100;

		//			unitPoints->dragoonLastValue		= unitPoints->dragoon;
		//			unitPoints->dragoon					= 100;

		//			unitPoints->zealotLastValue			= unitPoints->zealot;
		//			unitPoints->zealot					= 50;

		//			unitPoints->darkTemplarLastValue	= unitPoints->darkTemplar;
		//			unitPoints->darkTemplar				= 20;
		//		}
		//		else if (unitPoints->corsair == 100)
		//		{
		//			unitPoints->corsair		= unitPoints->corsairLastValue;
		//			unitPoints->dragoon		= unitPoints->dragoonLastValue;
		//			unitPoints->zealot		= unitPoints->zealotLastValue;
		//			unitPoints->darkTemplar = unitPoints->darkTemplarLastValue;
		//		}
		//	}

		//	if (BWAPI::Broodwar->enemy()->getRace() == BWAPI::Races::Protoss)
		//	{
		//		if (myCorsairs < nbScout + nbCarrier)
		//		{
		//			unitPoints->corsairLastValue		= unitPoints->corsair;
		//			unitPoints->corsair					= 100;

		//			unitPoints->dragoonLastValue		= unitPoints->dragoon;
		//			unitPoints->dragoon					= 100;

		//			unitPoints->zealotLastValue			= unitPoints->zealot;
		//			unitPoints->zealot					= 50;

		//			unitPoints->darkTemplarLastValue	= unitPoints->darkTemplar;
		//			unitPoints->darkTemplar				= 20;
		//		}
		//		else if (unitPoints->corsair == 100)
		//		{
		//			unitPoints->corsair		= unitPoints->corsairLastValue;
		//			unitPoints->dragoon		= unitPoints->dragoonLastValue;
		//			unitPoints->zealot		= unitPoints->zealotLastValue;
		//			unitPoints->darkTemplar = unitPoints->darkTemplarLastValue;
		//		}
		//	}

		//	if (BWAPI::Broodwar->enemy()->getRace() == BWAPI::Races::Zerg)
		//	{
		//		if (myCorsairs < nbMuta + nbDevo)
		//		{
		//			unitPoints->corsairLastValue		= unitPoints->corsair;
		//			unitPoints->corsair					= 100;

		//			unitPoints->dragoonLastValue		= unitPoints->dragoon;
		//			unitPoints->dragoon					= 100;

		//			unitPoints->zealotLastValue			= unitPoints->zealot;
		//			unitPoints->zealot					= 50;

		//			unitPoints->darkTemplarLastValue	= unitPoints->darkTemplar;
		//			unitPoints->darkTemplar				= 20;
		//		}
		//		else if (unitPoints->corsair == 100)
		//		{
		//			unitPoints->corsair		= unitPoints->corsairLastValue;
		//			unitPoints->dragoon		= unitPoints->dragoonLastValue;
		//			unitPoints->zealot		= unitPoints->zealotLastValue;
		//			unitPoints->darkTemplar = unitPoints->darkTemplarLastValue;
		//		}
		//	}
		//}
	}
}





