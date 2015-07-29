#pragma once

#include <IMTerrain.h>

using namespace BWAPI;
using namespace std;
//--------------------
IMTerrain::IMTerrain(int v, int r):InfluenceMap(IM_OCCUPANCE)
{
	//params = GA::getInstance()->getParams();
	//params_range = GA::getInstance()->getParamsRange();

	//value = 100;
	//radius = 7;

	value = v;
	radius = r;
}

void IMTerrain::Init(int sizeX, int sizeY, int wSizeX, int wSizeY){
	InfluenceMap::Init(sizeX, sizeY, wSizeX, wSizeY);
	this->init();
}

void IMTerrain::init()
{
	memset(m_map,0,m_numCels*sizeof(int));

	for(int i =0;i< m_numCels;i++){
		int gridY = i/m_dataSizeX;
		int gridX = i%m_dataSizeX;
		int posX = gridX*m_celResX + m_celResX/2;
		int posY = gridY*m_celResY + m_celResY/2;
		Position p(posX, posY);
		//set the value in the wall to be a high value		
		if(this->isUnwalkable(p)){
			m_map[i] = this->value;
		}
	}

	int range = radius;

	while(range-- > 0){
		int hv = this->value * (range+1)/radius;
		int lv = this->value * range/radius;
		//set the value neer the wall
		for(int i =0;i< m_numCels;i++){
			if(m_map[i] >= hv) continue;
			if(this->isNearby(i, hv)){
				m_map[i] = lv;
			}
		}	
	}
}

void IMTerrain::Update(float dt)
{
}


//check in BWTA
bool IMTerrain::isUnwalkable(BWAPI::Position pos){
	int pave = 8;
	Position ppp = Position(pos.x()/pave, pos.y()/pave);
	for(std::set<BWTA::Polygon*>::const_iterator r=BWTA::getUnwalkablePolygons().begin();r!=BWTA::getUnwalkablePolygons().end();r++){
		BWTA::Polygon* pol=*r;
		
		bool in = pol->isInside(ppp);
		if(in){
			return true;
		}
	}
	return false;
}

bool IMTerrain::isNearby(int grid, int targetValue){
	int gridY = grid/m_dataSizeX;
	int gridX = grid%m_dataSizeX;
	
	for(int i=gridX-1;i<=gridX+1;i++){
		for(int j=gridY-1; j<=gridY+1;j++){
			if(i<0) i=0;
			if(j<0) j=0;
			if(i> m_dataSizeX) i= m_dataSizeX;
			if(j> m_dataSizeX) j= m_dataSizeX;
			if(i== gridX && j== gridY) continue;  //skip the cell which the unit is inside
			int value = this->GetInfluenceValue(m_map, i, j);
			if(value == targetValue){
				return true;
			}
		}
	}
	return false;
}
//--------------------
void IMTerrain::RegisterGameObj(Unit* object)
{
}





//--------------------
void IMTerrain::DrawTheInfluence()
{
	if (!debugMode) return;

	DrawTheGrid();
	DrawGridValue();
}

//--------------------
void IMTerrain::DrawGridValue()
{
	for(int i=0;i<m_numCels;i++)
	{
		if(m_map[i] == 0) continue;

		int gridX = i%m_dataSizeX;
		int gridY = i/m_dataSizeX;
		int x = gridX*m_celResX+m_celResX/3;
		int y = gridY*m_celResY+m_celResY/3;
		
		BWAPI::Broodwar->drawTextMap(x,y,"%d",m_map[i]);
	}
}

IMTerrain::~IMTerrain()
{
	
}

