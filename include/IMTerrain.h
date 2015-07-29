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

#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include <InfluenceMap.h>
#include "GA.h"

class IMTerrain:public InfluenceMap
{
public:
	IMTerrain(int v, int r);
	~IMTerrain();
	virtual void Init(int sizeX, int sizeY, int wSizeX, int wSizeY);
	virtual void Update(float dt);
	virtual void RegisterGameObj(BWAPI::Unit* object);
	virtual void DrawTheInfluence();
	virtual void DrawGridValue();

	void init();
	//check in BWTA
	bool isUnwalkable(BWAPI::Position p);
	bool isNearby(int grid, int value);

private:
	int value;
	int radius;
};