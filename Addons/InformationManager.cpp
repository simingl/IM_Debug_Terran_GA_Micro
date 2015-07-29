#include <InformationManager.h>
#include "Util.h"
using namespace std;
using namespace BWAPI;

InformationManager* TheInformationManager = NULL;

InformationManager* InformationManager::create()
{
  if (TheInformationManager) return TheInformationManager;
  return new InformationManager();
}
void InformationManager::destroy()
{
  if (TheInformationManager)
    delete TheInformationManager;
}

IMSelfSum* InformationManager::getIMSelfSum()
{
	return this->sIMsum;
}

InformationManager::InformationManager()
{

  params = GA::getInstance()->getParams();
  params_range = GA::getInstance()->getParamsRange();


  this->tIM = new IMTerrain(params.TerrainValue, params.TerrainRadius);
  this->tIM_Range = new IMTerrain(params_range.TerrainValue, params_range.TerrainRadius);

  this->eIM = new IMEnemy(params.UnitValue, params.UnitRadius);
  this->eIM_Range = new IMEnemy(params_range.UnitValue, params_range.UnitRadius);

  this->sIMsum = new IMSelfSum();

  int mapWidth  = Broodwar->mapWidth();
  int mapHeight = Broodwar->mapHeight();

  int sizeX = mapWidth;
  int sizeY = mapHeight;
  int unitSize = 32;

  this->tIM->Init(sizeX,sizeY,sizeX*unitSize,sizeY*unitSize);
  this->eIM->Init(sizeX,sizeY,sizeX*unitSize,sizeY*unitSize);
  this->eIM->setTerrainIM(this->tIM->GetIMValue());

  this->tIM_Range->Init(sizeX,sizeY,sizeX*unitSize,sizeY*unitSize);
  this->eIM_Range->Init(sizeX,sizeY,sizeX*unitSize,sizeY*unitSize);
  this->eIM_Range->setTerrainIM(this->tIM_Range->GetIMValue());

  this->sIMsum->Init(sizeX,sizeY,sizeX*unitSize,sizeY*unitSize);

  TheInformationManager = this;
  buildTime[Broodwar->enemy()->getRace().getCenter()]=0;
  buildTime[Broodwar->enemy()->getRace().getWorker()]=0;
  if (Broodwar->enemy()->getRace()==Races::Zerg)
  {
    buildTime[UnitTypes::Zerg_Larva]=0;
    buildTime[UnitTypes::Zerg_Overlord]=0;
  }
  startLocationCouldContainEnemy = BWTA::getStartLocations();
  startLocationCouldContainEnemy.erase(BWTA::getStartLocation(Broodwar->self()));
  scoutedAnEnemyBase = false;
  if (startLocationCouldContainEnemy.size()==1)
  {
    enemyBases.insert(*startLocationCouldContainEnemy.begin());
    scoutedAnEnemyBase = true;
  }
  for each(Unit* u in Broodwar->getAllUnits())
  {
    onUnitDiscover(u);
  }

  
}
InformationManager::~InformationManager()
{
  TheInformationManager = NULL;
  delete this->eIM;
  delete this->sIMsum;

}
void InformationManager::onUnitDiscover(Unit* unit)
{
  if (!(Broodwar->self()->isEnemy(unit->getPlayer()) ||(unit->getPlayer() == Broodwar->self()))  ) return;
  savedData[unit].exists = true;
  int time=Broodwar->getFrameCount();
  UnitType type=unit->getType();
  updateBuildTime(type,time-type.buildTime());
  if (scoutedAnEnemyBase == false && unit->getType().isBuilding())
  {
    BWTA::Region* r=BWTA::getRegion(unit->getTilePosition());
    if (r->getBaseLocations().size()==1)
    {
      BWTA::BaseLocation* b = *(r->getBaseLocations().begin());
      enemyBases.insert(b);
      scoutedAnEnemyBase = true;
    }
  }
  if (unit->getType().isResourceDepot())
  {
    BWTA::BaseLocation* b=BWTA::getNearestBaseLocation(unit->getTilePosition());
    enemyBases.insert(b);
    enemyBaseCenters[b]=unit;
    scoutedAnEnemyBase = true;
  }
  //register this unit to influence map
  if(!unit->getType().isBuilding())
  {
	this->eIM->RegisterGameObj(unit);
	this->eIM_Range->RegisterGameObj(unit);

	this->sIMsum->RegisterGameObj(unit);
  }
}
void InformationManager::onUnitEvade(Unit* unit)
{
	if (!(Broodwar->self()->isEnemy(unit->getPlayer()) ||(unit->getPlayer() == Broodwar->self()))  ) return;
	
  savedData[unit].player=unit->getPlayer();
  savedData[unit].type=unit->getType();
  savedData[unit].typeName=unit->getType().getName();
  savedData[unit].position=unit->getPosition();
  savedData[unit].lastSeenTime=Broodwar->getFrameCount();
}

void InformationManager::onUnitDestroy(Unit* unit)
{
  if (!(Broodwar->self()->isEnemy(unit->getPlayer()) ||(unit->getPlayer() == Broodwar->self()))  ) return;
  this->onUnitEvade(unit);
  savedData[unit].exists=false;
  
  if (unit->getType().isResourceDepot())
  {
    BWTA::BaseLocation* b=BWTA::getNearestBaseLocation(unit->getTilePosition());
    if (this->enemyBaseCenters[b]==unit)
    {
      this->enemyBases.erase(b);
      this->enemyBaseCenters.erase(b);
    }
  }

  //remove unit in influence map
  this->eIM->RemoveGameObj(unit);
  this->eIM_Range->RemoveGameObj(unit);
  //this->eIMtank->RemoveGameObj(unit);
  this->sIMsum->getIMmarine()->RemoveGameObj(unit);
//  this->sIMsum->getIMtank()->RemoveGameObj(unit);
}

Player* InformationManager::getPlayer(Unit* unit) const
{
  if (unit->exists())
    return unit->getPlayer();
  map<Unit*,UnitData>::const_iterator i=savedData.find(unit);
  if (i==savedData.end())
    return NULL;
  return (*i).second.player;
}

UnitType InformationManager::getType(Unit* unit) const
{
  if (unit->exists())
    return unit->getType();
  map<Unit*,UnitData>::const_iterator i=savedData.find(unit);
  if (i==savedData.end())
    return UnitTypes::None;
  return (*i).second.type;
}

Position InformationManager::getLastPosition(Unit* unit) const
{
  if (unit->exists())
    return unit->getPosition();
  map<Unit*,UnitData>::const_iterator i=savedData.find(unit);
  if (i==savedData.end())
    return Positions::None;
  return (*i).second.position;
}

int InformationManager::getLastSeenTime(Unit* unit) const
{
  if (unit->exists())
    return Broodwar->getFrameCount();
  map<Unit*,UnitData>::const_iterator i=savedData.find(unit);
  if (i==savedData.end())
    return -1;
  return (*i).second.lastSeenTime;
}

bool InformationManager::exists(Unit* unit) const
{
  if (unit->exists())
    return true;
  map<Unit*,UnitData>::const_iterator i=savedData.find(unit);
  if (i==savedData.end())
    return false;
  return (*i).second.exists;
}

bool InformationManager::enemyHasBuilt(UnitType type) const
{
  return (buildTime.find(type)!=buildTime.end());
}

int InformationManager::getBuildTime(UnitType type) const
{
  map<UnitType, int>::const_iterator i=buildTime.find(type);
  if (i==buildTime.end())
    return -1;
  return i->second;
}

const set<BWTA::BaseLocation*>& InformationManager::getEnemyBases() const
{
  return this->enemyBases;
}

void InformationManager::setBaseEmpty(BWTA::BaseLocation* base)
{
  this->startLocationCouldContainEnemy.erase(base);
  if (startLocationCouldContainEnemy.size()==1)
  {
    enemyBases.insert(*startLocationCouldContainEnemy.begin());
    scoutedAnEnemyBase = true;
  }
}

void InformationManager::updateBuildTime(UnitType type, int time)
{
  map<UnitType, int>::iterator i=buildTime.find(type);
  if (i!=buildTime.end() && (i->second<=time || i->second==0)) return;
  buildTime[type]=time;
  if (time<0) return;
  for(map< UnitType,int>::const_iterator i=type.requiredUnits().begin();i!=type.requiredUnits().end();i++)
  {
    updateBuildTime(i->first,time-i->first.buildTime());
  }
}

InformationManager::UnitData::UnitData()
{
  position     = Positions::Unknown;
  type         = UnitTypes::Unknown;
  player       = NULL;
  lastSeenTime = -1;
  exists       = false;
}


void InformationManager::Update()
{
	this->eIM->Update(1);
	this->eIM_Range->Update(1);
	
}