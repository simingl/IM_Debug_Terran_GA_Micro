#pragma once

#include <IMEnemy.h>
#include <cstring>
#include <sstream>  
#include <string> 
#include <algorithm>
using namespace BWAPI;
using namespace std;
//--------------------
IMEnemy::IMEnemy(int _value, int _radius):InfluenceMap(IM_OCCUPANCE)
{
	//params = GA::getInstance()->getParams();
	//params_range = GA::getInstance()->getParamsRange();

	//value = 30;
	//radius = 13;

	value = _value;
	radius = _radius;
}

void IMEnemy::Init(int sizeX, int sizeY, int wSizeX, int wSizeY){
	InfluenceMap::Init(sizeX, sizeY, wSizeX, wSizeY);
	this->m_map_tr = new int[m_numCels];
	//initial 0
	memset(m_map_tr,0,m_numCels*sizeof(int));
}

void IMEnemy::Update(float dt)
{
	
	//bail out if nobody to update
	if(registeredObjects.size() == 0)
		return;
	//based on terrain IM map.
	std::memcpy(m_map, m_map_tr, m_numCels*sizeof(int));

	//stamp obj locations
	std::map<BWAPI::Unit*, RegObj>::iterator listObj;
	int count = 0;
	for(listObj=registeredObjects.begin();listObj!=registeredObjects.end();++listObj)
	{
		if (!listObj->second.exist) continue;  

		int posX = listObj->first->getPosition().x();
		int posY = listObj->first->getPosition().y();
		
		if (posX > m_worldSizeX) posX = listObj->second.m_lastPosX;
		if (posY > m_worldSizeY) posY = listObj->second.m_lastPosY;

		float hp = listObj->first->getHitPoints()+listObj->first->getShields();
		int hp_max = listObj->first->getType().maxHitPoints() + listObj->first->getShields();
		float hp_ratio = hp/hp_max;
		//unit's weight added according to hp ratio.
		StampInfluenceGradientSum(m_map,posX, posY, hp_ratio*value, radius);
	}
}

//--------------------
void IMEnemy::RegisterGameObj(Unit* object)
{
	if(object->getPlayer() == Broodwar->self())     //self, ignore
		return;

	registeredObjects[object].m_lastPosX = object->getPosition().x();
	registeredObjects[object].m_lastPosY = object->getPosition().y();
	registeredObjects[object].m_stamped = false;
	registeredObjects[object].m_objType = object->getType().getID();
	registeredObjects[object].m_typeName = object->getType().getName();
	registeredObjects[object].exist = true;
	registeredObjects[object].isBuilding = object->getType().isBuilding();
	
	registeredObjects[object].m_objSide = -1;
}

std::set<int> IMEnemy::getBorlderCellList(int ringCount)
{
	std::set<int> border;
	for(int i=0;i<m_numCels;i++)
	{
		if(ringCount ==1)
		{
			if(m_map[i] == 0 && hasEnemyInRange(m_map, i, 1))
				border.insert(i);
		}else if(ringCount==2)
		{
			if(m_map[i] == 0 && hasEnemyInRange(m_map, i, 2) && !hasEnemyInRange(m_map, i, 1))
				border.insert(i);
		}else if(ringCount ==3)
		{
			if(m_map[i] == 0 && hasEnemyInRange(m_map, i, 3)&& !hasEnemyInRange(m_map, i, 2) && !hasEnemyInRange(m_map, i, 1))
				border.insert(i);
		}
	}
	return border;
}



//--------------------
void IMEnemy::DrawTheInfluence()
{
	if (!debugMode) return;
	if(registeredObjects.size() == 0)
		return;

	//DrawTheGrid();
	//DrawGridValue();
	DrawUnit();
}

void IMEnemy::DrawUnit(){
	for (std::map<BWAPI::Unit*, RegObj>::iterator i= registeredObjects.begin(); i!= registeredObjects.end();i++){
		BWAPI::Broodwar->drawCircleMap(i->first->getPosition().x(), i->first->getPosition().y(),5,Colors::Red);
	}
}

BWAPI::Unit* IMEnemy::getLowestValueUnit(){
	BWAPI::Unit* t;
	int min = 100000;
	for (std::map<BWAPI::Unit*, RegObj>::iterator i= registeredObjects.begin(); i!= registeredObjects.end();i++){
		int value = this->GetInfluenceValue(m_map, i->first->getPosition());
		if(value < min){
			min = value;
			t = i->first;
		}
	}
	return t;
}

BWAPI::Position IMEnemy::getHidingPos(Position* pos, int celldis){
	int grid = this->getGridFromPosition(*pos);
	for (int i = 0;i< celldis; i++){
		grid = this->getLowestNearbyGrid(grid);
		int value = m_map[grid];
		if(value <= 0){
			break;
		}
	}
	return this->getPositionFromGrid(grid);
}

BWAPI::Position IMEnemy::getLowestNearby(BWAPI::Unit* u){
	int grid = getGridFromPosition(u->getPosition());
	if(grid > m_dataSizeX*m_dataSizeX)
		return Position(0,0);
	int count = 0;
	while (this->GetInfluenceValueByGrid(m_map, grid) > 0 && count++ < m_worldSizeX){
		grid = getLowestNearbyGrid(grid);
		Position p = this->getPositionFromGrid(grid);
		BWAPI::Broodwar->drawCircleMap(p.x(), p.y(),3,Colors::Yellow, true);
	}

	return this->getPositionFromGrid(grid);
}

int IMEnemy::getLowestNearbyGrid(int grid){
	int gridY = grid/m_dataSizeX;
	int gridX = grid%m_dataSizeX;
	
	int min = 100000;
	int gridNumber = grid;
	int mX = min(gridX+1, m_dataSizeX-1);
	int mY = min(gridY+1, m_dataSizeX-1);
	for(int i=gridX-1;i<=mX;i++){
		for(int j=gridY-1; j<=mY;j++){
			if(i<0) i=0;
			if(j<0) j=0;
			if(i== gridX && j== gridY) continue;  //skip the cell which the unit is inside
			int value = this->GetInfluenceValue(m_map, i, j);
			if(value < min){
				min = value;
				gridNumber = j*m_dataSizeX + (i%m_dataSizeX);
			}
		}
	}
	
	return gridNumber;
}

void IMEnemy::setTerrainIM(int* m){
	std::memcpy(m_map_tr, m, m_numCels*sizeof(int));
}
//--------------------
void IMEnemy::DrawGridValue()
{
	
	for(int i=0;i<m_numCels;i++)
	{
		int gridX = i%m_dataSizeX;
		int gridY = i/m_dataSizeX;
		int x = gridX*m_celResX+m_celResX/2;
		int y = gridY*m_celResY+m_celResY/2;
		
		if (m_map[i]==0) continue;

		BWAPI::Broodwar->drawTextMap(x-10,y-10,"%d",m_map[i]);
		//BWAPI::Broodwar->drawTextMap(x-10,y-10,"%d",i);
	}
	int total = 0;
	std::map<BWAPI::Unit*, RegObj>::iterator listObj;
	for(listObj=registeredObjects.begin();
		listObj!=registeredObjects.end();++listObj)
	{
		if (listObj->second.exist) total++ ;
	}

	int cells  = m_numCels;
	int cellWidth = m_celResX;
	int cellHeight = m_celResY;
	char color = '\x01';

	//Broodwar->drawTextScreen(250,20, "Total   : %d",total);
	//Broodwar->drawTextScreen(250,30, "Width   : %d",m_dataSizeX);
	//Broodwar->drawTextScreen(250,40, "Height  : %d",m_dataSizeY);
	//Broodwar->drawTextScreen(250,50, "Cells   : %d",cells);
	//Broodwar->drawTextScreen(250,60, "CellW   : %d",cellWidth);
	//Broodwar->drawTextScreen(250,70, "CellH   : %d",cellHeight);

	if (BWAPI::Broodwar->getFrameCount() % 20 == 0){
		stringstream width;
		stringstream height;
		stringstream data;
		string w;
		string h;
		string d;
		
		width << "width="<<m_dataSizeX;
		height << "height="<<m_dataSizeY;
		data <<"data=";
		for (int i=0;i<m_numCels;i++)
		{
			data << m_map[i];
			data <<",";
		}

		width >> w;
		height >> h;
		data >> d;
		write2file(w,h,d);
	}
}

IMEnemy::~IMEnemy()
{
	
}

void IMEnemy::write2file(string w, string h, string d)
{
	const char filename[] = "c:\\lsm\\im.properties";
	ofstream o_file;
	ifstream i_file;

	//o_file.open(filename, ios::app);
	o_file.open(filename);
	o_file << w << endl; 
	o_file << h << endl; 
	o_file << d << endl; 
	o_file.close();
}