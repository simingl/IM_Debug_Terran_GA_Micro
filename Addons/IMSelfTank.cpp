#pragma once

#include <IMSelfTank.h>

using namespace BWAPI;
using namespace std;
//--------------------
IMSelfTank::IMSelfTank():InfluenceMap(IM_OCCUPANCE)
{
}


void IMSelfTank::Update(float dt)
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
		if (!listObj->second.exist) continue;  

		int posX = listObj->first->getPosition().x();
		int posY = listObj->first->getPosition().y();
		
		if (posX > m_worldSizeX) posX = listObj->second.m_lastPosX;
		if (posY > m_worldSizeY) posY = listObj->second.m_lastPosY;

		int influenceValue = -1000;
		int influenceRadius = 1;

		StampInfluenceGradientSum(m_map,posX, posY, influenceValue,influenceRadius);
	}
}

void IMSelfTank::Update(std::vector<int> cells)
{
	memset(m_map,0,m_numCels*sizeof(int));
	int value = 500;
	for(std::vector<int>::iterator i = cells.begin(); i!= cells.end(); i++)
	{
		m_map[*i] = value;
		value -= 2;
	}

	this->Update(0.1);
}
//--------------------
void IMSelfTank::RegisterGameObj(Unit* object)
{
	if(object->getPlayer() != Broodwar->self())     //self, ignore
		return;

	if(!(object->getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode 
		|| object->getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Siege_Mode))     //no tank, ignore
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

BWAPI::Position IMSelfTank::getAttractionPoint()
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
	return getPositionFromGrid(pos);
}

//--------------------
void IMSelfTank::DrawTheInfluence()
{
	//if (!debugMode) return;
	//if(registeredObjects.size() == 0)
	//	return;

	DrawTheGrid();
	DrawGridValue();
}

//--------------------
void IMSelfTank::DrawGridValue()
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

IMSelfTank::~IMSelfTank()
{
	
}

void IMSelfTank::write2file(string w, string h, string d)
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