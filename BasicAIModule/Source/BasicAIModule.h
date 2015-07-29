#pragma once
#include <BWAPI.h>
#include <BWTA.h>
#include <Arbitrator.h>
#include <TechManager.h>
#include <UpgradeManager.h>
#include <BaseManager.h>
#include <InformationManager.h>
#include <BorderManager.h>
#include <UnitGroupManager.h>
#include <EnhancedUI.h>
#include <ArmyManager.h>
#include <IMEnemy.h>
#include <IMTerrain.h>
#include <IMSelfSum.h>
#include "GA.h"

class BasicAIModule : public BWAPI::AIModule
{
public:
  virtual void onStart();
  virtual void onEnd(bool isWinner);
  virtual void onFrame();
  virtual void onUnitDiscover(BWAPI::Unit* unit);
  virtual void onUnitEvade(BWAPI::Unit* unit);
  virtual void onUnitMorph(BWAPI::Unit* unit);
  virtual void onUnitRenegade(BWAPI::Unit* unit);
  virtual void onUnitDestroy(BWAPI::Unit* unit);
  virtual void onSendText(std::string text);
  ~BasicAIModule(); //not part of BWAPI::AIModule
  void showStats(); //not part of BWAPI::AIModule
  void showPlayers();
  void showForces(bool show);
  bool analyzed;
  std::map<BWAPI::Unit*,BWAPI::UnitType> buildings;
  Arbitrator::Arbitrator<BWAPI::Unit*,double> arbitrator;
  
  TechManager* techManager;
  UpgradeManager* upgradeManager;
  
  InformationManager* informationManager;
  BorderManager* borderManager;
  UnitGroupManager* unitGroupManager;
  ArmyManager* armyManager;
  InfluenceMap* influenceMap;

  EnhancedUI* enhancedUI;
  bool showManagerAssignments;
  
  IMEnemy* eIM;
  IMTerrain* tIM;
  IMSelfSum* sIMsum;
};
