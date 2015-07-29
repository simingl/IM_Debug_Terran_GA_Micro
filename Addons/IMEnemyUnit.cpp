#pragma once

#include <IMEnemyUnit.h>

using namespace BWAPI;
using namespace std;
//--------------------
IMEnemyMarine::IMEnemyMarine():InfluenceMap(IM_OCCUPANCE)
{
	params = GA::getInstance()->getParams();

	value = params.MarineValue;
	radius = params.MarineRadius;
}

void IMEnemyMarine::Update(float dt)
{
	//bail out if nobody to update
	if(registeredObjects.size() == 0)
		return;
	//clear out map
	memset(m_map,0,m_numCels*sizeof(int));
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

		StampInfluenceGradientSum(m_map,posX, posY, -1*value, radius);
	}
}

//--------------------
void IMEnemyMarine::RegisterGameObj(Unit* object)
{
	if(object->getPlayer() == Broodwar->self() || object->getType() != BWAPI::UnitTypes::Terran_Marine)     //self, ignore
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

BWAPI::Position IMEnemyMarine::getMarineCenter()
{
	std::set<int> groups = this->getEnemyGroups();
	if(groups.size() <= 0) return BWAPI::Position(0,0);

	int x = 0;
	int y = 0;

	for(std::set<int>::iterator g = groups.begin(); g!= groups.end(); g++)
	{
		x += this->getPositionFromGrid(*g).x();
		y += this->getPositionFromGrid(*g).y();
	}
	
	x = x/groups.size();
	y = y/groups.size();
	
	return BWAPI::Position(x,y);
}

std::set<int> IMEnemyMarine::getEnemyGroups()
{
	std::set<int> groups;
	for(int i=0;i<m_numCels;i++)
	{
		int fff = m_map[i];
		if(m_map[i] < 0) //enemy
		{
			int gridX = i%m_dataSizeX;
			int gridY = i/m_dataSizeX;
			int startX = gridX - 3;
			int startY = gridY - 3;
			int stopX  = gridX + 3;
			int stopY  = gridY + 3;
			startX = startX<0?0:startX;
			startY = startY<0?0:startY;
			stopX = stopX>=m_dataSizeX?m_dataSizeX:stopX;
			stopY = stopY>=m_dataSizeY?m_dataSizeY:stopY;
			int pos = i;
			int min = 0;
			for(int row =startY;row <stopY; row++)
			{
				for(int col =startX; col < stopX; col++)
				{
					int index = row*m_dataSizeX + (col%m_dataSizeX);
					int xxx = m_map[index];
					int yyy = m_map[i];
					if(m_map[index] < m_map[i])
					{
						pos = index;
						break;
					}
				}
				if(i != pos) break;
			}
			if(i==pos)
			{
				if(groups.size() > 0)
				{
					int minDist = 10000;
					for(std::set<int>::iterator g=groups.begin(); g != groups.end(); g++)
					{
						int group = (*g);
						int gridXG = i%m_dataSizeX;
						int gridYG = i/m_dataSizeX;
						int dist = this->getDistanceFrom2Grid(group, i);
						if(minDist > dist)
						{
							minDist = dist;
						}
					}
					if(minDist > 3*32)
					{
						groups.insert(i);
					}
				}
				else
				{
					groups.insert(i);
				}
			}
		}
	}
	return groups;
}

int IMEnemyMarine::getNextTargetGroup()
{
	std::set<int> groups = this->getEnemyGroups();
	int mystrong = this->getMyStrongGrid();
	int minDist = 100000;
	int minGroup = -1;
	for(std::set<int>::iterator g = groups.begin(); g!= groups.end(); g++)
	{
		int dist = this->getDistanceFrom2Grid(*g, mystrong);
		if(dist < minDist)
		{
			minDist = dist;
			minGroup = *g;
		}
	}
	return minGroup;
}

BWAPI::Position IMEnemyMarine::getMyStrongPoint()
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

int IMEnemyMarine::getMyStrongGrid()
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
	return pos;
}

std::set<int> IMEnemyMarine::getBorlderCellList(int ringCount)
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

std::vector<int> IMEnemyMarine::getGroupBorlderCellList(int groupCenter, std::set<int> borderCellList)
{
	std::set<int> groups = this->getEnemyGroups();
	for (std::set<int>::iterator i = borderCellList.begin(); i!=borderCellList.end();i++)
	{
		int dist = getDistanceFrom2Grid(*i, groupCenter);
		int minDist = 100000;
		for(std::set<int>::iterator g = groups.begin(); g!=groups.end(); g++)
		{
			minDist = min(minDist, getDistanceFrom2Grid(*i, *g));
		}
		if(abs(dist - minDist) > 10)
		{
			borderCellList.erase(*i);
		}
	}
	
	std::vector<int> v;
	int size = borderCellList.size();
	for(int i= 0; i< size;i++)
	{
		std::set<int>::iterator b = borderCellList.begin();
		int middle = borderCellList.size()/2;
		for(int j=0;j<middle;j++) b++;
		v.push_back(*b);
		borderCellList.erase(*b);
	}

	return v;
}

//--------------------
void IMEnemyMarine::DrawTheInfluence()
{
	if (!debugMode) return;
	if(registeredObjects.size() == 0)
		return;

	DrawTheGrid();
	DrawGridValue();
}

//--------------------
void IMEnemyMarine::DrawGridValue()
{
	
	for(int i=0;i<m_numCels;i++)
	{
		int gridX = i%m_dataSizeX;
		int gridY = i/m_dataSizeX;
		int x = gridX*m_celResX+m_celResX/2;
		int y = gridY*m_celResY+m_celResY/2;

		BWAPI::Broodwar->drawTextMap(x-10,y-10,"\07%d",m_map[i]);
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

IMEnemyMarine::~IMEnemyMarine()
{
	
}

void IMEnemyMarine::write2file(string w, string h, string d)
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