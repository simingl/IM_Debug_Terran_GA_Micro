#pragma once

#include <IMSelfMarine.h>

using namespace BWAPI;
using namespace std;
//--------------------
IMSelfMarine::IMSelfMarine():InfluenceMap(IM_OCCUPANCE)
{
	params = GA::getInstance()->getParams();
}

void IMSelfMarine::Update(float dt)
{
	//bail out if nobody to update
	if(registeredObjects.size() == 0)
		return;
	//clear out map
	//memset(m_map,0,m_numCels*sizeof(int));
	//stamp obj locations
	std::map<BWAPI::Unit*, RegObj>::iterator listObj;
	int count = 0;
	for(listObj=registeredObjects.begin();listObj!=registeredObjects.end();++listObj)
	{
		if (!listObj->second.exist) 
		{
			continue; 
		}
		
		int posX = listObj->first->getPosition().x();
		int posY = listObj->first->getPosition().y();
		
		if (posX > m_worldSizeX) posX = listObj->second.m_lastPosX;
		if (posY > m_worldSizeY) posY = listObj->second.m_lastPosY;

		int influenceValue = -500;

		int index = this->getGridFromPosition(listObj->first->getPosition());
		
		if(index > m_numCels) continue;

		m_map[index] += influenceValue;
		
		if (true)
		{
			BWAPI::Broodwar->drawTextMap(listObj->first->getPosition().x(), listObj->first->getPosition().y(), "\x07 %d", listObj->second.currentGrid);
		}
	}
}

void IMSelfMarine::Update(std::vector<int> cells)
{
	memset(m_map,0,m_numCels*sizeof(int));
	int value = 500;
	for(std::vector<int>::iterator i = cells.begin(); i!= cells.end(); i++)
	{
		m_map[*i] = value;
		value -= 5;
	}
	this->Update(0.1);
}

//--------------------
void IMSelfMarine::RegisterGameObj(Unit* object)
{
	if(object->getPlayer() != Broodwar->self() || object->getType() != BWAPI::UnitTypes::Terran_Marine)     //self, ignore
		return;

	registeredObjects[object].m_lastPosX = object->getPosition().x();
	registeredObjects[object].m_lastPosY = object->getPosition().y();
	registeredObjects[object].m_stamped = false;
	registeredObjects[object].m_objType = object->getType().getID();
	registeredObjects[object].m_typeName = object->getType().getName();
	registeredObjects[object].exist = true;
	registeredObjects[object].isBuilding = object->getType().isBuilding();
	registeredObjects[object].currentGrid = -1;
	
	registeredObjects[object].m_objSide = 1;
}

int IMSelfMarine::getAttractionPoint()
{
	int max = 0;
	int pos = 0;

	for(int i=0;i<m_numCels;i++)
	{
		if(m_map[i] > max) 
		{
			max=m_map[i];
			pos = i;
		}
	}
	if(pos == 406)
		int xxx = m_map[406];
	if(max == 500)
		int xxx = 0;
	return pos;
}

bool IMSelfMarine::isCellOcupied(int cell)
{
	std::map<BWAPI::Unit*, RegObj>::iterator listObj;
	for(listObj=registeredObjects.begin();listObj!=registeredObjects.end();++listObj)
	{
		if(listObj->second.exist && listObj->second.currentGrid == cell)
			return true;
	}
	return false;
}

BWAPI::Unit* IMSelfMarine::getOcupiedUnitFromCell(int cell)
{
	std::map<BWAPI::Unit*, RegObj>::iterator listObj;
	for(listObj=registeredObjects.begin();listObj!=registeredObjects.end();++listObj)
	{
		if(listObj->second.exist && listObj->second.currentGrid == cell)
			return listObj->first;
	}
	return NULL;
}

void IMSelfMarine::ocupiedUnitinCell(BWAPI::Unit* unit, int cell)
{
	registeredObjects[unit].currentGrid = cell;
}

bool IMSelfMarine::isUnitOcupied(BWAPI::Unit* unit)
{
	int current = registeredObjects[unit].currentGrid;
	if(registeredObjects[unit].exist && registeredObjects[unit].currentGrid > 0)
	{
		return true;
	}
	return false;
}

bool IMSelfMarine::isUnitInPosition(BWAPI::Unit* unit)
{
	if(isUnitOcupied(unit))
	{
		int cell = registeredObjects[unit].currentGrid;
		Position p = this->getPositionFromGrid(cell);
		if(p.getApproxDistance(unit->getPosition()) <=32)
			return true;
	}
	return false;
}

Position IMSelfMarine::getUnitOcupiedPosition(Unit* unit)
{
	if(isUnitOcupied(unit))
	{
		int cell = registeredObjects[unit].currentGrid;
		return this->getPositionFromGrid(cell);
	}
}
//--------------------
void IMSelfMarine::DrawTheInfluence()
{
	//if (!debugMode) return;
	//if(registeredObjects.size() == 0)
	//	return;

	DrawTheGrid();
	DrawGridValue();

}

//--------------------
void IMSelfMarine::DrawGridValue()
{
	
	for(int i=0;i<m_numCels;i++)
	{
		int gridX = i%m_dataSizeX;
		int gridY = i/m_dataSizeX;
		int x = gridX*m_celResX+m_celResX/2;
		int y = gridY*m_celResY+m_celResY/2;

		BWAPI::Broodwar->drawTextMap(x,y,"%d",m_map[i]);
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
	char color = '\x07';

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

IMSelfMarine::~IMSelfMarine()
{
	
}

void IMSelfMarine::write2file(string w, string h, string d)
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

