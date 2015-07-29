#pragma once

#include <BWTA.h>
#include <BWAPI.h>
#include <algorithm>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include "GA.h"
#include "IMParams.h"
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include <InfluenceMap.h>

class IMEnemyMarine:public InfluenceMap
{
public:
	IMEnemyMarine();
	~IMEnemyMarine();
	virtual void Update(float dt) ;
	virtual void RegisterGameObj(BWAPI::Unit* object);
	virtual void DrawTheInfluence();
	virtual void DrawGridValue();
	std::vector<int> getGroupBorlderCellList(int groupCenter, std::set<int> borderCellList);
	std::set<int> getBorlderCellList(int ringCount);
	BWAPI::Position getMarineCenter();
	std::set<int> getEnemyGroups();
	int getNextTargetGroup();

	BWAPI::Position getMyStrongPoint();
	int getMyStrongGrid();
	BWAPI::Position getDefencePoint();

private:
	void write2file(std::string w, std::string h, std::string d);

	IMParams params;
	int value;
	int radius;

};