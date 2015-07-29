#include "Squad.h"

using namespace std;
using namespace BWAPI;

Squad::Squad()
{
	params = GA::getInstance()->getParams();

	totalDPS=0; id=-1; totalHP = 0; mode=Idle;
	
	//A = 1000;            //Attraction
	//B = 300000;          //Nearby repulsion
	C = 25000000;        //Enemy repulsion
	l = 3.0;             //Enemy repulsion
	//m = 3.0;             //Nearby repulsion
	//n = 1.2;             //Attraction
	A = params.A*100;
	B = params.B*10000;
	//C = params.C*1000000;
	//l = params.l*0.3;
	m = params.m*0.3;
	n = params.n*0.3;


	repulsionThreshold = 1000;
}

void Squad::setTargetPosition(BWAPI::Position position)
{
	target = position;
}

const BWAPI::Position Squad::getTargetPosition() const
{
	return target;
}

int Squad::getSquadHP()
{
	int totalHP = 0;
	for (std::set<Unit*>::iterator i = goliaths.begin(); i!=goliaths.end(); i++)
	{

		totalHP += (*i)->getHitPoints();
	}

	for (std::set<Unit*>::iterator i = vultures.begin(); i!=vultures.end(); i++)
	{
		totalHP += (*i)->getHitPoints();
	}

	for (std::set<BWAPI::Unit*>::iterator t = tanks.begin(); t != tanks.end(); t++ )
	{
		totalHP += (*t)->getHitPoints();
	}
	return totalHP;
}

BWAPI::Position* Squad::getSquadPosition()
{
	return &currentPosition;
}

void Squad::setSquadID(int squadId)
{
	id = squadId;
}
void Squad::setInformationManager(InformationManager* am)
{
	this->informationManager = am;
	this->eIM = informationManager->getIMEnemy();
	this->sIMsum  = informationManager->getIMSelfSum();
}
void Squad::holdPosition(BWAPI::Position target)
{
	this->mode = Squad::SquadMode::Idle;

	BWAPI::Position patrolPosition(target.x()+10, target.y()+10);
	for (std::set<BWAPI::Unit*>::iterator u=tanks.begin(); u!= tanks.end(); u++)
	{
		(*u)->attack(BWAPI::Position(target.x()-170, target.y()-170));
		(*u)->patrol(BWAPI::Position(target.x()+170, target.y()+170), true);

		if ((*u)->getPosition().getDistance(target) <= 200 && (*u)->getPosition().getDistance(target) >= 170 &&
			(*u)->getType()==BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode)
		{
			(*u)->useTech(TechTypes::Tank_Siege_Mode);
		}
		else if (((*u)->getPosition().getDistance(target) > 200 ||(*u)->getPosition().getDistance(target) < 170) &&
			(*u)->getType()==BWAPI::UnitTypes::Terran_Siege_Tank_Siege_Mode)
		{
			(*u)->useTech(TechTypes::Tank_Siege_Mode);
		}
	}

	for (std::set<BWAPI::Unit*>::iterator u=goliaths.begin(); u!= goliaths.end(); u++)
	{
		(*u)->attack(BWAPI::Position(target.x()-130, target.y()-130));
		(*u)->patrol(BWAPI::Position(target.x()+130, target.y()+130), true);
	}
}

Unit* Squad::getTargetInSightRange(Unit* unit)
{
	std::set<BWAPI::Unit*> units = unit->getUnitsInRadius(unit->getType().sightRange());
	int minDist = 10000;
	Unit* target;
	for(set<Unit*>::iterator u = units.begin();u != units.end(); u++)
	{
		if (!((*u)->getPlayer() == BWAPI::Broodwar->self()))
		{
			int dist = (*u)->getPosition().getDistance(unit->getPosition());
			if( dist < minDist)
			{
				minDist = dist;
				target = *u;
			}
		}
	}
	if(minDist == 10000)
		return NULL;
	else
		return target;
}

void Squad::attackPosition(BWAPI::Position target)
{
	this->target = target;

	this->mode = Squad::SquadMode::Attacking;

	for (std::set<Unit*>::iterator i = goliaths.begin(); i!=goliaths.end(); i++)
	{
		(*i)->attack(target);
	}

	for (std::set<Unit*>::iterator i = vultures.begin(); i!=vultures.end(); i++)
	{
		(*i)->attack(target);
	}

	for (std::set<Unit*>::iterator i = marines.begin(); i!=marines.end(); i++)
	{
		Unit* enemy = getTargetInSightRange(*i);
		if(enemy != NULL)
		{
			if(!((*i)->isInWeaponRange(enemy)))
				(*i)->attack(enemy);
		}
		else
			this->potentialMove(*i, &target, false);
	}

	for (std::set<BWAPI::Unit*>::iterator t = tanks.begin(); t != tanks.end(); t++ )
	{
		Unit* enemy = getTargetInSightRange(*t);
		if(enemy != NULL)
		{
			if(!((*t)->isInWeaponRange(enemy)))
				(*t)->attack(enemy);
		}
		else
			this->potentialMove(*t, &target, false);
	}
}

void Squad::addMarine(BWAPI::Unit* u)
{
	marines.insert(u);
	allUnits.insert(u);
}

void Squad::addGoliath(BWAPI::Unit* u)
{
	goliaths.insert(u);
	allUnits.insert(u);
}
void Squad::addVulture(BWAPI::Unit* u)
{
	vultures.insert(u);
	allUnits.insert(u);
}
void Squad::addTanks(BWAPI::Unit* u)
{
	tanks.insert(u);
	allUnits.insert(u);
}
int Squad::getTanksCount()
{
	return tanks.size();
}
int Squad::getGoliathCount()
{
	return goliaths.size();
}
int Squad::getVultureCount()
{
	return vultures.size();
}

int Squad::getSquadScore()
{
	int mykillscore = BWAPI::Broodwar->self()->getKillScore();
	int enemykillscore = 8*100 + 700 - this->marines.size() * 100 - this->tanks.size() * 700;

	double frame = BWAPI::Broodwar->getFrameCount();
	//Time score is the within [0-1) * marine score
	double timescore = BWAPI::UnitTypes::Terran_Marine.destroyScore() * (1 - frame/2500);    //2500 is the set longest time in one game
	
	double score = mykillscore - enemykillscore;

	if(score < 0) timescore *= -1;  //when the enemy win, time score is negative
	score += timescore;

//	if(score > -1 && score < 1)
//	{
//		Position tankCenter = this->eIMtank->getTanksCenter();
//		Position marineCenter = this->eIM->getMarineCenter();
//		Position center = Position((tankCenter.x()+marineCenter.x())/2, (tankCenter.y()+marineCenter.y())/2);
//		int minDist = 100000;
//		int distance = 0;
//		for(std::set<Unit*>::iterator i = this->allUnits.begin(); i!= allUnits.end();i++)
//		{
//			int dist = (*i)->getDistance(center);
//			distance += dist;
//			minDist = min(minDist, dist);
//		}
//		float m = (1 - ((float)minDist)/4096)*100;
//		float d = ((1-(float)distance/allUnits.size()/4096)*100);
//		score = m;
//	}

	return score;
}
void Squad::updateCurrentPosition()
{
	int x = 0;
	int y = 0;
	int count = 0 ;

	for (std::set<Unit*>::iterator i = tanks.begin(); i!=tanks.end(); i++)
	{
		x += (*i)->getPosition().x();
		y += (*i)->getPosition().y();
		count++;
	}

	for (std::set<Unit*>::iterator i = goliaths.begin(); i!=goliaths.end(); i++)
	{
		x += (*i)->getPosition().x();
		y += (*i)->getPosition().y();
		count++;
	}
    
	if (count == 0)
	{
		return;
	}
	else
	{
		currentPosition = BWAPI::Position(x/count, y/count);
	}
}

void Squad::removeMember(BWAPI::Unit* u)
{
	if (goliaths.find(u) != goliaths.end())
	{
		goliaths.erase(u);
	}
	else if (tanks.find(u) != tanks.end())
	{
		tanks.erase(u);
	}else if (vultures.find(u) != vultures.end())
	{
		vultures.erase(u);
	}else if(marines.find(u) != marines.end())
	{
		marines.erase(u);
	}

	if(allUnits.find(u) != allUnits.end())
	{
		allUnits.erase(u);
	}

}

void Squad::draw()
{
	this->updateCurrentPosition();
	std::string cp = "C Point";
	std::string tar = "TARGET";
	char color = '\x07';

	BWAPI::Broodwar->drawCircleMap(this->getSquadPosition()->x(),this->getSquadPosition()->y(), 20, BWAPI::Colors::Red);
	BWAPI::Broodwar->drawTextMap(this->getSquadPosition()->x(),this->getSquadPosition()->y(),"%c:%s %d",color, cp.c_str(), id);
	

	if (target.x()>0 && target.y()>0)
	{
		BWAPI::Broodwar->drawCircleMap(this->target.x(), this->target.y(), 30, BWAPI::Colors::Orange);
		BWAPI::Broodwar->drawTextMap(this->target.x(),this->target.y(),"%c:%s %d",color, tar.c_str(), id);
	}

	for (std::set<Unit*>::iterator i=marines.begin(); i!= marines.end(); i++)
	{
		BWAPI::Broodwar->drawTextMap((*i)->getPosition().x(),(*i)->getPosition().y(),"%c: %d",color,id);
	}

	for (std::set<Unit*>::iterator i=medics.begin(); i!= medics.end(); i++)
	{
		BWAPI::Broodwar->drawTextMap((*i)->getPosition().x(),(*i)->getPosition().y(),"%c: %d",color,id);
		if (i==medics.begin())
		{
			BWAPI::Broodwar->drawLineMap((*i)->getPosition().x(),(*i)->getPosition().y(), this->target.x(),this->target.y(), BWAPI::Colors::Orange);
		}
	}

	for (std::set<Unit*>::iterator i=tanks.begin(); i!= tanks.end(); i++)
	{
		BWAPI::Broodwar->drawTextMap((*i)->getPosition().x(),(*i)->getPosition().y(),"%c: %d",color, id);
	}
}

void Squad::updateSquadFormation()
{

}

Squad::SquadMode Squad::getSquadMode()
{
	return mode;
}

int Squad::getDistance(BWAPI::Position* p1, BWAPI::Position* p2)
{
	return sqrt((float)((p1->x()-p2->x())*(p1->x()-p2->x()) + (p1->y()-p2->y())*(p1->y()-p2->y())));
}

BWAPI::Position Squad::getTempFormationPosition(BWAPI::Position* unitPos, BWAPI::Position* squadPos, int distance)
{
	int tempDistance = distance;
	double degree = atan2((float)(squadPos->y()-unitPos->y()), (float)(squadPos->x()-unitPos->x()));
	int x = tempDistance*cos(degree)+unitPos->x();
	int y = tempDistance*sin(degree)+unitPos->y();
	return BWAPI::Position(x,y);
}


bool Squad::isSquadMember(BWAPI::Unit* u)
{
	if (goliaths.find(u) != goliaths.end())
	{
		return true;
	}
	else if (tanks.find(u) != tanks.end())
	{
		return true;
	}	
	else if (vultures.find(u) != vultures.end())
	{
		return true;
	}else if (marines.find(u) != marines.end())
	{
		return true;
	}

	return false;
}

void Squad::battleFormation(BWAPI::Position defencePoint, BWAPI::Position enemyPoint)
{
	BWAPI::Broodwar->drawCircleMap(enemyPoint.x(),enemyPoint.y(), 30, BWAPI::Colors::Red,true);

	int xEnemy = enemyPoint.x();
	int yEnemy = enemyPoint.y();
	int xDef = defencePoint.x() - xEnemy;
	int yDef = defencePoint.y() - yEnemy;
	int r = defencePoint.getDistance(enemyPoint);
	float sita = 3.14 - atan ((float)yDef/(float)xDef);
	float gap = 50;
	int rowGap = 85;
	float diffGoliath = gap/r;
	float diffVulture = gap/(r-rowGap);
	float diffTank = gap/(r+rowGap);
	int goliathsSize = goliaths.size();
	int vulturesSize = vultures.size();
	int tanksSize    = tanks.size();
	int countGoliath = -1*goliathsSize/2;
	int countVulture = -1*vulturesSize/2;
	int countTanks = -1*tanksSize/2;

	for (std::set<Unit*>::iterator t = tanks.begin(); t!=tanks.end(); t++)
	{
		float angle = sita+countTanks*diffTank*2;
		countTanks += 2;
		int x = (r+rowGap)*cos(angle)+xEnemy;
		int y = (r+rowGap)*sin(angle)+yEnemy;
		
		BWAPI::Broodwar->drawCircleMap(x,y, 10, BWAPI::Colors::Blue,true);
		(*t)->attack(BWAPI::Position(x,y));
	}

	for (std::set<Unit*>::iterator i = vultures.begin(); i!=vultures.end(); i++)
	{
		float angle = sita+countVulture*diffVulture;
		countVulture++;
		int x = (r-rowGap)*cos(angle)+xEnemy;
		int y = (r-rowGap)*sin(angle)+yEnemy;
		
		BWAPI::Broodwar->drawCircleMap(x,y, 10, BWAPI::Colors::Green,true);
		(*i)->attack(BWAPI::Position(x,y));
	}

	for (std::set<Unit*>::iterator i = goliaths.begin(); i!=goliaths.end(); i++)
	{
		float angle = sita+countGoliath*diffGoliath*1.2;
		countGoliath++;
		int x = r*cos(angle)+xEnemy;
		int y = r*sin(angle)+yEnemy;
		
		BWAPI::Broodwar->drawCircleMap(x,y, 10, BWAPI::Colors::Red,true);
		(*i)->attack(BWAPI::Position(x,y));
	}
}

void Squad::battleAttackFormation(BWAPI::Position defencePoint, BWAPI::Position enemyPoint)
{
	BWAPI::Broodwar->drawCircleMap(enemyPoint.x(),enemyPoint.y(), 30, BWAPI::Colors::Red,true);

	int xEnemy = enemyPoint.x();
	int yEnemy = enemyPoint.y();
	int xDef = defencePoint.x() - xEnemy;
	int yDef = defencePoint.y() - yEnemy;
	int r = defencePoint.getDistance(enemyPoint);
	float sita = 3.14 - atan ((float)yDef/(float)xDef);
	float gap = 50;
	int rowGap = 85;
	float diffGoliath = gap/r;
	float diffVulture = gap/(r-rowGap);
	float diffTank = gap/(r+rowGap);
	int goliathsSize = goliaths.size();
	int vulturesSize = vultures.size();
	int tanksSize    = tanks.size();
	int countGoliath = -1*goliathsSize/2;
	int countVulture = -1*vulturesSize/2;
	int countTanks = -1*tanksSize/2;

	for (std::set<Unit*>::iterator t = tanks.begin(); t!=tanks.end(); t++)
	{
		float angle = sita+countTanks*diffTank*2;
		countTanks += 2;
		int x = (r+rowGap)*cos(angle)+xEnemy;
		int y = (r+rowGap)*sin(angle)+yEnemy;
		
		BWAPI::Broodwar->drawCircleMap(x,y, 10, BWAPI::Colors::Blue,true);
		(*t)->attack(BWAPI::Position(x,y));
	}

	for (std::set<Unit*>::iterator i = vultures.begin(); i!=vultures.end(); i++)
	{
		float angle = sita+countVulture*diffVulture;
		countVulture++;
		int x = (r-rowGap)*cos(angle)+xEnemy;
		int y = (r-rowGap)*sin(angle)+yEnemy;
		
		BWAPI::Broodwar->drawCircleMap(x,y, 10, BWAPI::Colors::Green,true);
		(*i)->attack(BWAPI::Position(x,y));
	}

	for (std::set<Unit*>::iterator i = goliaths.begin(); i!=goliaths.end(); i++)
	{
		float angle = sita+countGoliath*diffGoliath*1.2;
		countGoliath++;
		int x = r*cos(angle)+xEnemy;
		int y = r*sin(angle)+yEnemy;
		
		BWAPI::Broodwar->drawCircleMap(x,y, 10, BWAPI::Colors::Red,true);
		(*i)->attack(BWAPI::Position(x,y));
	}
}

bool Squad::isSquadInPosition()
{
	bool isInPosition = true;
	for (std::set<Unit*>::iterator m = marines.begin(); m!=marines.end(); m++)
	{
		bool flag = this->sIMsum->getIMmarine()->isUnitInPosition(*m);
		if(!flag)
			isInPosition = false;
	}
	return isInPosition;
}

bool Squad::battleAttackFormation()
{
	return true;
}

void Squad::potentialMove(Unit* unit, Position* target, bool withBlock)
{
	BWAPI::Broodwar->drawCircleMap(target->x(),target->y(), 30, BWAPI::Colors::Green);


	int distanceFromTarget = unit->getPosition().getDistance(*target);
	int distanceRange = 15;

	if(distanceFromTarget < distanceRange)//arrived 
	{
		unit->stop(); 
		return;
	}

	vmml::vector<2, double> r, u, Fs, Pfs, potentialVec;
	vmml::vector<2, double> vecPos1, vecPos2, vecPos3;
	double vec_zero[] = {0,0};
	
	potentialVec = vec_zero;  //initialize

	double unitpos[] = {unit->getPosition().x(),unit->getPosition().y()};
	double targpos[] = {target->x(), target->y()};

	double repulsionPotential = 0.0;
	double attractioPotential = 0.0;
	double blockRepuPotential = 0.0;

	int nInRange = 1;
	//start to calculate repulsion
	for (std::set<Unit*>::iterator t = allUnits.begin(); t!=allUnits.end(); t++)
	{
		if(unit->getID() == (*t)->getID()) continue; //repulsed by all other entities
		int dist = unit->getPosition().getDistance((*t)->getPosition());
		if( dist < this->repulsionThreshold)  //Don't care about entities too far away
		{
			nInRange ++;
			double nearpos[] = {(*t)->getPosition().x(),(*t)->getPosition().y()};
			
			vecPos1 = unitpos;
			vecPos2 = nearpos;

			r = vecPos1 - vecPos2;
			u = r;
			u.normalize();
			repulsionPotential = B/pow(dist, m);
			potentialVec += u * repulsionPotential;
		}
	}
	//start to calculate block replusion --------------------
	if(withBlock)
	{
		//Position tankCenter = this->eIMtank->getTanksCenter();
		//Position marineCenter = this->eIM->getMarineCenter();
		//double block1[] = {tankCenter.x(), tankCenter.y()};
		//double block2[] = {marineCenter.x(), marineCenter.y()};
		//Position blockPos1(block1[0],block1[1]);
		//Position blockPos2(block2[0],block2[1]);
		//vecPos1 = unitpos;
		//vecPos2 = block1;
		//int distanceFromBlock = unit->getPosition().getDistance(blockPos1);
		//r = vecPos1-vecPos2;
		//r.normalize();
		////block1
		//blockRepuPotential += C/pow(distanceFromBlock, l);
		//potentialVec += r *blockRepuPotential * nInRange;
		////block2
		//vecPos2 = block2;
		//distanceFromBlock = unit->getPosition().getDistance(blockPos2);
		//r = vecPos1-vecPos2;
		//r.normalize();
		//blockRepuPotential += C/pow(distanceFromBlock, l);
		//potentialVec += r *blockRepuPotential * nInRange;
	}

	//start to calculate attraction --------------------
	vecPos1 = unitpos;
	vecPos2 = targpos;

	r = vecPos1 - vecPos2;
	r.normalize();

	attractioPotential = -A/pow(distanceFromTarget, n);

	float currentHP =  unit->getHitPoints();
	float initialHP =  unit->getType().maxHitPoints();

	float HPratio =currentHP/initialHP*0.4+0.6;

	potentialVec += r * attractioPotential * nInRange * HPratio;
	if(distanceFromTarget > 200)
		potentialVec *= 100;
	else 
		potentialVec *= 10;
	int x =unit->getPosition().x() + potentialVec.x();
	int y =unit->getPosition().y() + potentialVec.y();
	
	unit->move(BWAPI::Position(x, y));
}

