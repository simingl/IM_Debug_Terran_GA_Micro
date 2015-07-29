#pragma once
#include <BWAPI.h>
#include <BWTA.h>
#include <map>
#include <IMTerrain.h>
#include <IMEnemy.h>
#include <IMSelfSum.h>
#include "IMParams.h"

using namespace std;

class InformationManager
{
  public:
    static InformationManager* create();
    static void destroy();
    void onUnitDiscover(BWAPI::Unit* unit);
    void onUnitEvade(BWAPI::Unit* unit);
    void onUnitDestroy(BWAPI::Unit* unit);
    BWAPI::Player* getPlayer(BWAPI::Unit* unit) const;
    BWAPI::UnitType getType(BWAPI::Unit* unit) const;
    BWAPI::Position getLastPosition(BWAPI::Unit* unit) const;
    int getLastSeenTime(BWAPI::Unit* unit) const;
    bool exists(BWAPI::Unit* unit) const;
    bool enemyHasBuilt(BWAPI::UnitType type) const;
    int getBuildTime(BWAPI::UnitType type) const;
    const std::set<BWTA::BaseLocation*>& getEnemyBases() const;
    void setBaseEmpty(BWTA::BaseLocation* base);
	IMEnemy* getIMEnemy(){return this->eIM;};
	IMEnemy* getIMEnemyRange(){return this->eIM_Range;};
	IMSelfSum* getIMSelfSum();
	void Update();

  private:
    InformationManager();
    ~InformationManager();
    class UnitData
    {
      public:
        UnitData();
        BWAPI::Position position;
        BWAPI::UnitType type;
        BWAPI::Player* player;
        int lastSeenTime;
		std::string typeName;
        bool exists;
    };

    void updateBuildTime(BWAPI::UnitType type, int time);
    std::map<BWAPI::Unit*, UnitData> savedData;
    std::map<BWAPI::UnitType, int> buildTime;
    std::set<BWTA::BaseLocation*> enemyBases;
    std::map<BWTA::BaseLocation*,BWAPI::Unit*> enemyBaseCenters;
    std::set<BWTA::BaseLocation*> startLocationCouldContainEnemy;

    bool scoutedAnEnemyBase;

	IMEnemy* eIM;
	IMEnemy* eIM_Range;
	IMTerrain* tIM;
	IMTerrain* tIM_Range;
	IMTerrain* etIM;
	IMSelfSum* sIMsum;
	
	IMParams params;
	IMParams params_range;
};
extern InformationManager* TheInformationManager;