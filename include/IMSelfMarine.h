#pragma once

#include <InfluenceMap.h>
#include <BWTA.h>
#include <BWAPI.h>
#include <algorithm>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "IMParams.h"
#include <stdio.h>
#include <conio.h>
#include <tchar.h>

class IMSelfMarine:public InfluenceMap
{
public:
	IMSelfMarine();
	~IMSelfMarine();
	virtual void Update(float dt);
	virtual void RegisterGameObj(BWAPI::Unit* object);
	virtual void DrawTheInfluence();
	
	virtual void DrawGridValue();
	void Update(std::vector<int> cells);
	int getAttractionPoint();
	void ocupiedUnitinCell(BWAPI::Unit* unit, int cell);
	bool isUnitOcupied(BWAPI::Unit* unit);
	bool isUnitInPosition(BWAPI::Unit* unit);
	BWAPI::Position getUnitOcupiedPosition(BWAPI::Unit* unit);
	bool isCellOcupied(int cell);
	BWAPI::Unit* getOcupiedUnitFromCell(int cell);

private:
	void write2file(std::string w, std::string h, std::string d);

	
	IMParams params;
	

};