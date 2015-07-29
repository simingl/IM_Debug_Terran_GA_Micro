#pragma once
#include <Arbitrator.h>
#include <InformationManager.h>
#include <BaseManager.h>
#include <BWAPI.h>
#include "BaseManager/Base.h"

class ScoutManager : public Arbitrator::Controller<BWAPI::Unit*,double>
{
  public:
    class ScoutData
    {
      public:
        enum ScoutMode
        {
          Idle,
          Searching,
          Roaming,
          Harassing,
          Fleeing
        };
        ScoutData(){ mode = Idle; }
        BWTA::BaseLocation* target;
        ScoutMode mode;
    };
    ScoutManager(Arbitrator::Arbitrator<BWAPI::Unit*,double>* arbitrator);
    virtual void onOffer(std::set<BWAPI::Unit*> units);
    virtual void onRevoke(BWAPI::Unit* unit, double bid);
    virtual void update();
    void setInformationManager(InformationManager* infoManager);
	void setBaseManager(BaseManager* baseManager);

    virtual std::string getName() const;
    virtual std::string getShortName() const;
    void onRemoveUnit(BWAPI::Unit* unit);

    // Non-Controller methods.
    bool isScouting() const;
    void setScoutCount(int count);
    void setDebugMode(bool debugMode);

    std::map<BWAPI::Unit*, ScoutData> scouts;
    Arbitrator::Arbitrator<BWAPI::Unit*,double>* arbitrator;
    
    std::list<BWTA::BaseLocation*> baseLocationsToScout;
    std::set<BWTA::BaseLocation*> baseLocationsExplored;
	std::set<BWTA::BaseLocation*> baseLocationsToScan;
	std::vector<BWTA::BaseLocation*> baseLocationsToScanVector;
    BWTA::BaseLocation *myStartLocation;
        
  private:
    bool needMoreScouts() const;
    void requestScout(double bid);
    void addScout(BWAPI::Unit* unit);
    void updateScoutAssignments();
    void drawAssignments();
	void scanEnemyBase();
	void updateBasesNotOwn();
    InformationManager* informationManager;
	BaseManager* baseManager;
    
    size_t desiredScoutCount;
    int scoutingStartFrame;
	int scanBaseCount;
    bool debugMode;
};