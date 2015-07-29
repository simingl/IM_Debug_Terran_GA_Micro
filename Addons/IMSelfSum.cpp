#pragma once

#include <IMSelfSum.h>

using namespace BWAPI;
using namespace std;
//--------------------
IMSelfSum::IMSelfSum():InfluenceMap(IM_OCCUPANCE)
{
	sIMmarine = new IMSelfMarine();
//	sIMtank = new IMSelfTank();
}

void IMSelfSum::Init(int sizeX, int sizeY, int wSizeX, int wSizeY)
{
	InfluenceMap::Init(sizeX, sizeY, wSizeX, wSizeY);

	this->sIMmarine->Init(sizeX, sizeY, wSizeX, wSizeY);
//	this->sIMtank->Init(sizeX, sizeY, wSizeX, wSizeY);
}

void IMSelfSum::Update(float dt)
{
//	std::set<int>    enemyBorderCellList = this->eIMsum->getBorlderCellList(1);
//	if(enemyBorderCellList.size() <= 0) return;

//	std::vector<int> marineBorderCell = this->eIMsum->getGroupBorlderCellList(0, enemyBorderCellList);
//	std::vector<int> tanksBorderCell = this->eIMsum->getGroupBorlderCellList(1, enemyBorderCellList);

//	this->sIMmarine->Update(tanksBorderCell);
//	this->sIMtank->Update(marineBorderCell);
}

//--------------------
void IMSelfSum::RegisterGameObj(Unit* object)
{
	if(Broodwar->self()->isEnemy(object->getPlayer()))	//enemy, ignore
		return;
	
	this->sIMmarine->RegisterGameObj(object);
//	this->sIMtank->RegisterGameObj(object);
}


//--------------------
void IMSelfSum::DrawTheInfluence()
{
	if (!debugMode) return;

	DrawTheGrid();
	DrawGridValue();
}

//--------------------
void IMSelfSum::DrawGridValue()
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

IMSelfSum::~IMSelfSum()
{
	delete this->sIMmarine;
}

void IMSelfSum::write2file(string w, string h, string d)
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