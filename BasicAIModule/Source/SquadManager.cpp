
#include "SquadManager.h"
#include <vector>
#include <algorithm>

using namespace BWAPI;
using namespace std;

SquadManager::SquadManager(){
	this->needsregroup = false;
	//A = 1000;            //Attraction
	//B = 20000;          //Nearby repulsion
	//m = 3.0;             //Nearby repulsion
	//n = 1.2;             //Attraction

	params = GA::getInstance()->getParams();

	A = params.A * 100;            //Attraction
	B = params.B * 1000;          //Nearby repulsion
	m = params.m * 0.1;             //Nearby repulsion
	n = params.n * 0.1;             //Attraction

	repulsionThreshold = 500;
	repulsionThreshold = 1500;

	state = SquadState::Idle;
	this->im = InformationManager::create();
	this->target = NULL;
}

void SquadManager::loadUnits(){
	set<Unit*> allunits = Broodwar->getAllUnits();
	for each (Unit *u in allunits)
	{
		if(u->getPlayer() == Broodwar->self()){
			this->addUnit(u);
		}else if(u->getPlayer()->isEnemy(Broodwar->self())){
			this->addEnemy(u);
		}
	}
}
void SquadManager::addEnemy(Unit* unit){
	this->enemySet.insert(unit);
}

void SquadManager::addUnit(Unit* unit){
	MicroAspect *m = new MicroAspect(unit, this);
	this->unitSet.insert(unit);
	this->microAspectSet.insert(m);
	this->unitToMicroAspectMap[unit] = m;
}

void SquadManager::onUnitDestroy(Unit* unit){
	if(this->unitToMicroAspectMap.find(unit) != this->unitToMicroAspectMap.end()){
		MicroAspect *m = this->unitToMicroAspectMap[unit];
		this->unitSet.erase(unit);
		this->unitToMicroAspectMap.erase(unit);
		this->microAspectSet.erase(m);
		delete m;
	}else if(this->enemySet.find(unit) != this->enemySet.end()){
		this->enemySet.erase(unit);
	}
}

void SquadManager::onFrame(){

	if(Broodwar->getFrameCount() == 4){
		loadUnits();
	}
	
	//attack
	if(Broodwar->getFrameCount() > 5){
		//this->dodge();
		this->setTarget();
		this->refreshGroupPosition();
		this->onFire();
		this->draw();


	}
}

void SquadManager::onFire(){
	for(set<MicroAspect*>::const_iterator i=this->microAspectSet.begin();i!=this->microAspectSet.end();++i)
	{
		(*i)->onFire(this->enemySet);
	}
}

void SquadManager::dodge(){
	vector<Unit*> unitVec;
	copy(unitSet.begin(), unitSet.end(), std::back_inserter(unitVec));
	//sort(unitVec.begin(), unitVec.end(), hpcompare);
	
	int count = 0;
	for each (Unit* u in unitVec){
		int totalHP = u->getType().maxHitPoints() + u->getType().maxShields();
		int currHP  = u->getHitPoints() + u->getShields();
		if(count < unitVec.size()/2 && currHP < totalHP){
			this->unitToMicroAspectMap[u]->dodge(this->enemySet);
		}
	}
}

Unit* SquadManager::setTarget(){
	if(centerX>0 && centerY >0){
		Position cc(centerX, centerY);
		double minDist = 1000000;
		Unit* t;
		//calculate all the distances
		for each (Unit* u in this->enemySet){
			double dist = u->getPosition().getDistance(cc);
			if(dist< minDist){
				t = u;
				minDist= dist;
			}
		}
		this->target = t;

		return t;
	}else{
		this->target = NULL;
		return NULL;
	}
}

void SquadManager::draw(){
	Broodwar->drawTextScreen(100, 20,"%s%d", "SquadState: ", this->state );

	//Broodwar->drawCircleMap(centerX, centerY, 3, BWAPI::Colors::Green, true );
	//Broodwar->drawCircleMap(centerX, centerY, radius, BWAPI::Colors::White );

	Broodwar->drawTextScreen(20,10, "GA: %d,%d,%d,%d,,,%d,%d,%d,%d,,,%d,%d,%d,%d,%d,%d", params.UnitValue, params.UnitRadius,params.TerrainValue, params.TerrainRadius,
		params.A, params.B, params.m, params.n, params.m_freeze, params.m_kitingRange, params.m_targetNearby, params.m_kitingDist, params.m_hpfocus, params.m_hpkiting);

}

void SquadManager::refreshGroupPosition(){
	if(this->unitSet.size() == 0){
		centerX = 0;
		centerY = 0;
		radius = 1;
		return;
	}
	int totalX = 0;
	int totalY = 0;
	
	for each (Unit* u in this->unitSet){
		totalX += u->getPosition().x();
		totalY += u->getPosition().y();
	}
	
	centerX = totalX/this->unitSet.size();
	centerY = totalY/this->unitSet.size();
	
	Position cc(centerX,centerY);

	int dist = 0;
	for each (Unit* u in this->unitSet){
		dist += u->getDistance(cc);
	}

	radius = dist/this->unitSet.size();
}

void SquadManager::regroup(){
	for each (Unit* u in this->unitSet){
		u->move(Position(centerX, centerY));
	}
}

bool SquadManager::isSquadInCombat(){
	for each (Unit* u in this->unitSet){
		if(u->isAttacking() || u->isAttackFrame() || u->getGroundWeaponCooldown() > 0 || u->isUnderAttack()){
			return true;
		}
	}
	return false;
}

bool SquadManager::isSquadMoving(){
	for each (MicroAspect* m in this->microAspectSet){
		if(m->isUnitMoving()){
			return true;
		}
	}
	return false;
}


void SquadManager::potentialMove(Unit* unit, Position* target, int dist)
{
	BWAPI::Broodwar->drawCircleMap(target->x(),target->y(), 30, BWAPI::Colors::Green);

	int distanceFromTarget = unit->getPosition().getDistance(*target);
	int distanceRange = dist;

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
	for (std::set<Unit*>::iterator t = unitSet.begin(); t!=unitSet.end(); t++)
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
		potentialVec *= 50;
	else 
		potentialVec *= 2;

	int x =unit->getPosition().x() + potentialVec.x();
	int y =unit->getPosition().y() + potentialVec.y();
	
	Position dst(x,y);
	unit->move(dst);

	Broodwar->drawCircleMap(dst.x(), dst.y(),5,Colors::Orange, true);
	Broodwar->drawLineMap(unit->getPosition().x(), unit->getPosition().y(), dst.x(), dst.y(), Colors::Orange);
}

int SquadManager::getSquadScore()
{
	int unitscore = 100;
	int unitnumber = 5;
	int fu = this ->unitSet.size();
	int eu = this->enemySet.size();
	int du = fu - eu;

	double score = du*unitscore;

	double frame = BWAPI::Broodwar->getFrameCount();
	//Time score is the within [0-1) * marine score
	double timescore = BWAPI::UnitTypes::Terran_Marine.destroyScore() * (1 - frame/2500);    //2500 is the set longest time in one game
	
	if(score < 0) timescore *= -1;  //when the enemy win, time score is negative
	score += timescore;
	
	if(eu > 0 && fu > 0){    //didn't fight at all, punish
		score -= eu* unitscore;
	}

	return score;
}

int SquadManager::getSquadDestroyScore()
{
	double myscore = BWAPI::Broodwar->self()->getKillScore();
	double enscore = BWAPI::Broodwar->enemy()->getKillScore();
	return myscore - enscore;
}