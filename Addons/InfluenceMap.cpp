#include <InfluenceMap.h>
#include "Util.h"
using namespace std;
using namespace BWAPI;

InfluenceMap::InfluenceMap(int type) 
{
	m_influenceType = type;
}

InfluenceMap::~InfluenceMap()
{
	if(registeredObjects.size() == 0)
		return;
	registeredObjects.clear();
}

//--------------------
void InfluenceMap::Init(int sizeX, int sizeY, int wSizeX, int wSizeY)
{
	m_dataSizeX = sizeX;
	m_dataSizeY = sizeY;
	m_numCels = m_dataSizeX*m_dataSizeY;
	m_map = new int[m_numCels];
	//clear out the map
	memset(m_map,0,m_numCels*sizeof(int));
	m_worldSizeX = wSizeX;
	m_worldSizeY = wSizeY;
	m_celResX = m_worldSizeX / m_dataSizeX;
	m_celResY = m_worldSizeY / m_dataSizeY;

	debugMode = true;
}

//--------------------
void InfluenceMap::Reset()
{
	//clear out the map
	memset(m_map,0,m_numCels*sizeof(int));
	//get rid off all the objects
	if(registeredObjects.size() == 0)
		return;
	registeredObjects.clear();
}

//--------------------
void InfluenceMap::RegisterGameObj(BWAPI::Unit* object)
{
	registeredObjects[object].m_lastPosX = object->getPosition().x();
	registeredObjects[object].m_lastPosY = object->getPosition().y();
	registeredObjects[object].m_stamped = false;
	registeredObjects[object].exist = true;
}
//--------------------
void InfluenceMap::RemoveGameObj(BWAPI::Unit* object)
{
	if(registeredObjects.size() == 0)
		return;
	registeredObjects[object].exist = false;
	this->registeredObjects.erase(object);
}

//--------------------
int InfluenceMap::GetInfluenceValue(int* pMap,BWAPI::Position& location)
{
	int gridX = location.x()/ m_celResX;
	int gridY = location.y()/ m_celResY;

	int index = gridY*m_dataSizeX + (gridX%m_dataSizeX);

	return pMap[index];
}

//--------------------
int InfluenceMap::GetInfluenceValue(int* pMap,BWAPI::Position* location)
{
	int gridX = location->x()/ m_celResX;
	int gridY = location->y()/ m_celResY;

	int index = gridY*m_dataSizeX + (gridX%m_dataSizeX);

	return pMap[index];
}


int InfluenceMap::GetInfluenceValue(int* pMap,int gridX, int gridY){
	int index = gridY*m_dataSizeX + (gridX%m_dataSizeX);
	return pMap[index];
}

int InfluenceMap::GetInfluenceValueByGrid(int* pMap,int gridNumber){
	int gridY = gridNumber/m_dataSizeX;
	int gridX = gridNumber%m_dataSizeX;

	return GetInfluenceValue(pMap, gridX, gridY);
}

std::list<BWAPI::Position> InfluenceMap::GetInfluenceValueHigher(int* pMap,BWAPI::Position& location)
{
	list<BWAPI::Position> postionlist;
	int gridX = location.x()/m_celResX;
	int gridY = location.y()/m_celResY;

	int center = gridY*m_dataSizeX + (gridX%m_dataSizeX);
	for (int y=gridY-1;y<=gridY+1;y++)
	{
		for (int x=gridX-1;x<=gridX+1;x++)
		{
			int tmp = y*m_dataSizeX + (x%m_dataSizeX);
			if (pMap[tmp]>pMap[center])
			{
				int posX = x*m_celResX+m_celResX/2;
				int posY = y*m_celResY+m_celResY/2;
				postionlist.push_back(BWAPI::Position(posX,posY));
			}
		}
	}

	return postionlist;
}

//--------------------
void InfluenceMap::StampInfluenceGradient(int* pMap,int pos_x, int pos_y, int initValue)
{
	int gridX = floor((float)pos_x/ (float)m_celResX);
	int gridY = floor((float)pos_y/ (float)m_celResY);
	int stopDist = fabsf(initValue)*0.75f;
	int halfStopDist = stopDist / 2;
	int startX = gridX - halfStopDist;
	int stopX  = startX + stopDist;
	if(startX < 0) startX = 0;
	if(stopX > m_dataSizeX) stopX = m_dataSizeX;
	int startY = gridY - halfStopDist;
	int stopY  = startY + stopDist;
	if(startY < 0) startY = 0;
	if(stopY > m_dataSizeY) stopY = m_dataSizeY;
	for(int y = startY;y<stopY;y++)
	{
		for(int x = startX;x<stopX;x++)
		{	
			int value = 0;
			int distX = fabs((float)(x - ((gridX - halfStopDist) + halfStopDist)));
			int distY = fabs((float)(y - ((gridY - halfStopDist) + halfStopDist)));

			double destD = 0;
			if (!(x==gridX && y==gridY))
			{
				destD = sqrt((double)((x-gridX)*(x-gridX) + (y-gridY)*(y-gridY)));
			}
			
			int destI = ceil(destD);
			if (halfStopDist >= destI)
			{
				value += initValue*( halfStopDist - destI)/halfStopDist;
			}
			
			int index = y*m_dataSizeX + (x%m_dataSizeX);
			pMap[index] += value;
		}
	}
}

//--------------------
void InfluenceMap::StampInfluenceGradientSum(int* pMap,int pos_x, int pos_y, int initValue, int radius)
{
	int gridX = floor((float)pos_x/ (float)m_celResX);
	int gridY = floor((float)pos_y/ (float)m_celResY);
	int startX = gridX - radius;
	int stopX  = gridX + radius;
	if(startX < 0) startX = 0;
	if(stopX > m_dataSizeX) stopX = m_dataSizeX;
	int startY = gridY - radius;
	int stopY  = gridY + radius;
	if(startY < 0) startY = 0;
	if(stopY > m_dataSizeY) stopY = m_dataSizeY;
	for(int y = startY;y<stopY;y++)
	{
		for(int x = startX;x<stopX;x++)
		{	
			int value = 0;
			int distX = fabs((float)(x - gridX));
			int distY = fabs((float)(y - gridY));

			double destD = 0;
			if (!(x==gridX && y==gridY))
			{
				destD = sqrt((double)((x-gridX)*(x-gridX) + (y-gridY)*(y-gridY)));
			}
			
			int destI = ceil(destD);
			if (radius >= destI)
			{
				value += initValue*(radius-destI)/radius;
			}
			
			int index = y*m_dataSizeX + (x%m_dataSizeX);
			pMap[index] += value;
		}
	}
}

void InfluenceMap::DrawTheGrid()
{
	//BWAPI::Broodwar->setTextSize(2);
	for (int i=0; i< m_dataSizeX; i++)
	{
		BWAPI::Broodwar->drawLineMap(i*m_celResX,0,i*m_celResX, m_worldSizeY,BWAPI::Colors::Grey);
		BWAPI::Broodwar->drawTextMap(i*m_celResX,0,"%d",i);
	}

	for (int i=0; i< m_dataSizeY; i++)
	{
		BWAPI::Broodwar->drawLineMap(0,i*m_celResY,m_worldSizeX,i*m_celResY,BWAPI::Colors::Grey);
		BWAPI::Broodwar->drawTextMap(0, i*m_celResY,"%d",i);
	}	
}

void InfluenceMap::setDebugMode(bool debugMode)
{
	this->debugMode = debugMode;
}

BWAPI::Position InfluenceMap::getPositionFromGrid(int gridNumber)
{
	int gridX = gridNumber/m_dataSizeX;
	int gridY = gridNumber%m_dataSizeX;
	return this->getPositionFromGrid(gridX, gridY);
}

BWAPI::Position InfluenceMap::getPositionFromGrid(int row, int col)
{
	int pos_x = col*m_celResY + m_celResY/2;
	int pos_y = row*m_celResX + m_celResX/2;
	BWAPI::Position pos(pos_x, pos_y);
	return pos;	
}
int InfluenceMap::getGridFromPosition(BWAPI::Position position)
{
	int gridX = floor((float)position.x()/ (float)m_celResX);
	int gridY = floor((float)position.y()/ (float)m_celResY);
	int index = gridY*m_dataSizeX + (gridX%m_dataSizeX);
	return index;
}

int InfluenceMap::getDistanceFrom2Grid(int grid1, int grid2)
{
	BWAPI::Position p1 = getPositionFromGrid(grid1);
	BWAPI::Position p2 = getPositionFromGrid(grid2);
	return p1.getDistance(p2);
}
bool InfluenceMap::hasEnemyInRange(int* pMap, int gridNumber, int radius)
{
	int gridX = gridNumber/m_dataSizeX;
	int gridY = gridNumber%m_dataSizeX;
	int startX = gridX - radius;
	int stopX  = gridX + radius;
	if(startX < 0) startX = 0;
	if(stopX > m_dataSizeX-1) stopX = m_dataSizeX-1;
	int startY = gridY - radius;
	int stopY  = gridY + radius;
	if(startY < 0) startY = 0;
	if(stopY > m_dataSizeY-1) stopY = m_dataSizeY-1;
	//for(int y = startY; y <= stopY;y++)
	//{
	//	for(int x = startX;x <=stopX;x++)
	//	{	
	//		int index = x*m_dataSizeX + y;
	//		int value = pMap[index];
	//		if(value < 0 ) return true;
	//	}
	//}
	int indexU = startX*m_dataSizeX + gridY;
	int indexD = stopX* m_dataSizeX + gridY;
	int indexL = gridX *m_dataSizeX + startY;
	int indexR = gridX *m_dataSizeX + stopY;
	
	return pMap[indexU] < 0 || pMap[indexD] < 0 ||pMap[indexL] < 0 ||pMap[indexR] < 0;

	//return false;
}