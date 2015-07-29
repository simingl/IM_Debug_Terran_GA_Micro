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
#include <iostream>
#include <fstream>
#include "GA.h"
#include "IMParams.h"
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include <InfluenceMap.h>
#include <IMSelfMarine.h>



class IMSelfSum:public InfluenceMap
{
public:
	IMSelfSum();
	~IMSelfSum();
	virtual void Update(float dt);
	virtual void RegisterGameObj(BWAPI::Unit* object);
	virtual void DrawTheInfluence();
	virtual void DrawGridValue();

	void Init(int sizeX, int sizeY, int wSizeX, int wSizeY);
	IMSelfMarine * getIMmarine(){return this->sIMmarine;};
	int getHighestIMValue();

private:
	void write2file(std::string w, std::string h, std::string d);

	IMParams params;

	IMSelfMarine * sIMmarine;
};