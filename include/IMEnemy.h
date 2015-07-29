#pragma once

#include <BWTA.h>
#include <BWAPI.h>
#include <algorithm>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "GA.h"
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include <InfluenceMap.h>
#include <IMTerrain.h>

class IMEnemy:public InfluenceMap
{
public:
	IMEnemy(int _value, int _radius);
	~IMEnemy();
	virtual void Init(int sizeX, int sizeY, int wSizeX, int wSizeY);
	virtual void Update(float dt) ;
	virtual void RegisterGameObj(BWAPI::Unit* object);
	virtual void DrawTheInfluence();
	virtual void DrawGridValue();
	void DrawUnit();
	BWAPI::Unit* getLowestValueUnit();
	BWAPI::Position IMEnemy::getHidingPos(BWAPI::Position* pos, int celldis);
	BWAPI::Position getLowestNearby(BWAPI::Unit* u);
	int getLowestNearbyGrid(int gridNumber);
	std::set<int> getBorlderCellList(int ringCount);

	int getNextTargetGroup();
	void setTerrainIM(int* map);

private:
	void write2file(std::string w, std::string h, std::string d);

	
	int value;
	int radius;

	int* m_map_tr;

};