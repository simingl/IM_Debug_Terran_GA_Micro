#include "BasicAIModule.h"
#include "../Addons/Util.h"

using namespace BWAPI;

void BasicAIModule::onStart()
{
  Broodwar->setLocalSpeed(25);
  Broodwar->setGUI(true);
  this->showManagerAssignments=false;
  if (Broodwar->isReplay()) return;
  // Enable some cheat flags
  Broodwar->enableFlag(Flag::UserInput);
  Broodwar->enableFlag(Flag::CompleteMapInformation);
  BWTA::readMap();
  BWTA::analyze();
  this->analyzed=true;

  this->informationManager = InformationManager::create();
  this->borderManager      = BorderManager::create();
  this->unitGroupManager   = UnitGroupManager::create();
  this->armyManager        = new ArmyManager(&this->arbitrator);
  this->armyManager->setInformationManager(this->informationManager);
  this->enhancedUI         = new EnhancedUI();

  this->eIM = this->informationManager->getIMEnemy();
  this->sIMsum  = this->informationManager->getIMSelfSum();
}

BasicAIModule::~BasicAIModule()
{
  //delete this->techManager;
  //delete this->upgradeManager;
  delete this->armyManager;
  InformationManager::destroy();
  //BorderManager::destroy();
  UnitGroupManager::destroy();
  delete this->enhancedUI;
}
void BasicAIModule::onEnd(bool isWinner)
{
  log("onEnd(%d)\n",isWinner);

  int score = this->armyManager->getScore();

  ////write back to shared memory
  //GA::getInstance()->writeFitness2SharedMemory(score);

  BWAPI::Broodwar->printf("%s %d","Final Score is: ",score);
  
  BWAPI::Broodwar->restartGame();
}
void BasicAIModule::onFrame()
{
  if (Broodwar->isReplay()) return;
  if (!this->analyzed) return;
  this->enhancedUI->update();
  this->armyManager->update();
  this->arbitrator.update();

//  this->informationManager->getIMEnemySum()->Update(0.1);
  this->informationManager->Update();
  
  this->informationManager->getIMEnemy()->DrawTheInfluence();

  if(Broodwar->getFrameCount() == 2500)
  {
	  //write back to shared memory
	  int score = this->armyManager->getScore();
	  GA::getInstance()->writeFitness2SharedMemory(score);
	  //this->eIMMarine->getGA()->log2file(score);
	  BWAPI::Broodwar->restartGame();
	  //Broodwar->leaveGame();
  }
}

void BasicAIModule::onUnitDestroy(BWAPI::Unit* unit)
{
  if (Broodwar->isReplay()) return;
  this->arbitrator.onRemoveObject(unit);
  this->informationManager->onUnitDestroy(unit);
  this->armyManager->onRemoveUnit(unit);
}

void BasicAIModule::onUnitDiscover(BWAPI::Unit* unit)
{
  if (Broodwar->isReplay()) return;
  this->informationManager->onUnitDiscover(unit);
  this->unitGroupManager->onUnitDiscover(unit);
}
void BasicAIModule::onUnitEvade(BWAPI::Unit* unit)
{
  if (Broodwar->isReplay()) return;
  this->informationManager->onUnitEvade(unit);
  this->unitGroupManager->onUnitEvade(unit);
}

void BasicAIModule::onUnitMorph(BWAPI::Unit* unit)
{
  if (Broodwar->isReplay()) return;
  this->unitGroupManager->onUnitMorph(unit);
}
void BasicAIModule::onUnitRenegade(BWAPI::Unit* unit)
{
  if (Broodwar->isReplay()) return;
  this->unitGroupManager->onUnitRenegade(unit);
}

void BasicAIModule::onSendText(std::string text)
{
  if (Broodwar->isReplay())
  {
    Broodwar->sendText("%s",text.c_str());
    return;
  }
  UnitType type=UnitTypes::getUnitType(text);
  if (text=="debug")
  {
    if (this->showManagerAssignments==false)
    {
      this->showManagerAssignments=true;
	  this->informationManager->getIMEnemy()->setDebugMode(true);
	  this->informationManager->getIMSelfSum()->setDebugMode(true);
    }
    else
    {
      this->showManagerAssignments=false;
	  this->informationManager->getIMEnemy()->setDebugMode(false);
	  this->informationManager->getIMSelfSum()->setDebugMode(false);
    }
    Broodwar->printf("%s",text.c_str());
    return;
  }
  if (type!=UnitTypes::Unknown)
  {
    TechType type=TechTypes::getTechType(text);
    if (type!=TechTypes::Unknown)
    {
      this->techManager->research(type);
    }
    else
    {
      UpgradeType type=UpgradeTypes::getUpgradeType(text);
      if (type!=UpgradeTypes::Unknown)
      {
        this->upgradeManager->upgrade(type);
      }
      else
        Broodwar->printf("You typed '%s'!",text.c_str());
    }
  }
  Broodwar->sendText("%s",text.c_str());
}