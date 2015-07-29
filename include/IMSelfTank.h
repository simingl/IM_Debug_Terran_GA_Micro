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

class IMSelfTank:public InfluenceMap
{
public:
	IMSelfTank();
	~IMSelfTank();
	virtual void Update(float dt);
	virtual void RegisterGameObj(BWAPI::Unit* object);
	virtual void DrawTheInfluence();
	virtual void DrawGridValue();
	void Update(std::vector<int> cells);

	BWAPI::Position getAttractionPoint();
private:
	void write2file(std::string w, std::string h, std::string d);

	IMParams params;
};