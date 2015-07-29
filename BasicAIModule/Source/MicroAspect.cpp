#include "MicroAspect.h"
#include <cmath>
#include <map>
#include "SquadManager.h"

using namespace BWAPI;
using namespace std;

bool hpcompare  (Unit* i,Unit* j) { return (i->getHitPoints()+i->getShields() < j->getHitPoints()+j->getShields()); }

MicroAspect::MicroAspect(BWAPI::Unit* u, SquadManager* s){
	this->unit = u;
	this->squad = s;

	this->params = GA::getInstance()->getParams();
	this->params_range = GA::getInstance()->getParamsRange();

	//this->switchParams(true);

	microparam.freeze      = params.m_freeze;
	microparam.kitingRange = params.m_kitingRange   * 32;
	microparam.kitingDist  = params.m_kitingDist;   //IM cell distance
	microparam.targetNearby= params.m_targetNearby *10;
	microparam.hpfocus     = params.m_hpfocus           *0.1;
	microparam.hpkiting    = params.m_hpkiting         *0.1;

};

void MicroAspect::switchParams(bool isMelee){
	if(isMelee){
		microparam.freeze      = params.m_freeze;
		microparam.kitingRange = params.m_kitingRange   * 32;
		microparam.kitingDist  = params.m_kitingDist;   //IM cell distance
		microparam.targetNearby= params.m_targetNearby *10;
		microparam.hpfocus     = params.m_hpfocus           *0.1;
		microparam.hpkiting    = params.m_hpkiting         *0.1;
	}else{
		microparam.freeze      = params_range.m_freeze;
		microparam.kitingRange = params_range.m_kitingRange   * 32;
		microparam.kitingDist  = params_range.m_kitingDist;   //IM cell distance
		microparam.targetNearby= params_range.m_targetNearby *10;
		microparam.hpfocus     = params_range.m_hpfocus           *0.1;
		microparam.hpkiting    = params_range.m_hpkiting         *0.1;
	}
}

//Target selection
BWAPI::Unit* MicroAspect::getTarget(std::set<BWAPI::Unit*> &enemies){
	Unit* t = getNearestUnit(enemies);

	std::set<BWAPI::Unit*> nearbyunits = t->getUnitsInRadius(this->microparam.targetNearby);
	//remove friendly units
	for each(Unit* u in nearbyunits){
		if(u->getPlayer() == Broodwar->self()){
			nearbyunits.erase(u);
		}
	}
	
	Unit* low = NULL;   //check nearby enemy units, choose the low health target
	Unit* hf = NULL; 	//focus fire on target units.

	if(nearbyunits.size() > 0 ){
		low = getLowHPUnit(nearbyunits);
		hf  = getHighFocusUnit(nearbyunits);
	}
	
	if(low && this->getUnitHPPercent(low) < this->microparam.hpfocus){ //health < 0.3
		return low;
	}else if(hf && this->getNumberOfAttackers(hf) > 0){
		return hf;
	}else{
		return t;
	}
}


Unit* MicroAspect::getNearestUnit(std::set<BWAPI::Unit*> &enemies){
	double minDist = 100000;
	Unit* t;
	//calculate all the distances
	for each (Unit* u in enemies){
		double dist = unit->getDistance(u->getPosition());
		if(dist< minDist){
			t = u;
			minDist= dist;
		}
	}
	return t;
}

Unit* MicroAspect::getLowHPUnit(set<BWAPI::Unit*> &nearbyunits){
	vector<Unit*> unitVec;
	copy(nearbyunits.begin(), nearbyunits.end(), std::back_inserter(unitVec));
	sort(unitVec.begin(), unitVec.end(), hpcompare);
	
	return *(unitVec.begin());
}

Unit* MicroAspect::getHighFocusUnit(set<BWAPI::Unit*> &nearbyunits){
	int max = 0;
	Unit* hf;
	
	for each(Unit* u in nearbyunits){
		int n = this->getNumberOfAttackers(u);
		if (n >= max ){
			hf = u;
			max = n;
		}
	}
	
	return hf;
}

//return
int MicroAspect::getNumberOfAttackers(BWAPI::Unit* u){
	int count = 0;
	set<Unit*> units = this->squad->getSquadUnits();
	for each (Unit* unit in units){
		if(unit->getTarget() == u || unit->getOrderTarget() == u){
			count ++;
		}
	}
	return count;
}

void MicroAspect::onFire(set<Unit*> &enemies){
	//return;
	updatePosition();
	//no enemy
	if(enemies.size() <=0 ) return;

	Broodwar->drawTextMap(unit->getPosition().x(), unit->getPosition().y()-15,"%s%d", "cool ", unit->getGroundWeaponCooldown() );
	
	//attack freeze
	if((unit->getType().groundWeapon().damageCooldown()-unit->getGroundWeaponCooldown()) < this->microparam.freeze){
		return;
	}
	
	Unit* tt = this->getTarget(enemies);
	//bool target_shorter_weapon_range = tt->getType().groundWeapon().maxRange() < this->unit->getType().groundWeapon().maxRange();
	//this->switchParams(target_shorter_weapon_range);

	//ready to attack
	if(unit->getGroundWeaponCooldown()<=1){
		Unit* lowimunit = this->squad->im->getIMEnemy()->getLowestValueUnit();
		//update target
		bool isSquadMoving = this->squad->isSquadMoving();
		if(this->squad->state == SquadManager::SquadState::Approaching && !isSquadMoving){
			this->squad->switchState(SquadManager::SquadState::Attacking);
		}
		
		bool isSquadInCombat = this->squad->isSquadInCombat();
		if(isSquadInCombat){
			this->squad->switchState(SquadManager::SquadState::Attacking);
		}

		//if it is far from enemy, potential move toward enemy and formation
		if(unit->getDistance(tt) > unit->getType().sightRange() && (this->squad->state == SquadManager::SquadState::Approaching ||this->squad->state == SquadManager::SquadState::Idle)){
			this->squad->switchState(SquadManager::SquadState::Approaching);
			Position fmtp = this->squad->im->getIMEnemy()->getLowestNearby(lowimunit);
			this->squad->potentialMove(this->unit, &fmtp, 80);
			return;
		}

		if(unit->getOrder() != BWAPI::Orders::AttackUnit || target != tt ){
			this->target = tt;
			this->unit->attack(target);
		}
		return;
	}
	
	this->target = tt;
	kiteMove(enemies);
		
	draw();
}

void MicroAspect::kiteMove(set<Unit*> &enemies){
	float distance = unit->getDistance(target->getPosition());
	Position kitingpos = this->getKitingPositionByIM(&unit->getPosition());
	
	if(distance < target->getType().groundWeapon().maxRange() + this->microparam.kitingRange){
		if(this->target->getType().groundWeapon().maxRange() < this->unit->getType().groundWeapon().maxRange()){
			this->unit->move(kitingpos);
			//this->squad->potentialMove(this->unit, &kitingpos, 3);
			Broodwar->drawCircleMap(kitingpos.x(), kitingpos.y(),3,Colors::Cyan, true);
			Broodwar->drawLineMap(unit->getPosition().x(), unit->getPosition().y(), kitingpos.x(), kitingpos.y(), Colors::Cyan);
		}else if(this->isBeingTarget(unit, enemies) && (this->getUnitHPPercent(unit) < this->microparam.hpkiting)){ //kiting only being targeted
			this->unit->move(kitingpos);
			//this->squad->potentialMove(this->unit, &kitingpos, 3);
			Broodwar->drawCircleMap(kitingpos.x(), kitingpos.y(),3,Colors::Cyan, true);
			Broodwar->drawLineMap(unit->getPosition().x(), unit->getPosition().y(), kitingpos.x(), kitingpos.y(), Colors::Cyan);
		}
	}
}

void MicroAspect::draw(){
	//target circles ground weapon range, seek range, sight range.
	if(target){
		//Broodwar->drawCircleMap(target->getPosition().x(), target->getPosition().y(),target->getType().sightRange(),Colors::Orange);
		//Broodwar->drawCircleMap(target->getPosition().x(), target->getPosition().y(),target->getType().seekRange(),Colors::Orange);
		Broodwar->drawCircleMap(target->getPosition().x(), target->getPosition().y(),target->getType().groundWeapon().maxRange(),Colors::Orange);
		Broodwar->drawTextMap(target->getPosition().x(), target->getPosition().y(),"%d", this->getNumberOfAttackers(target) );

		Broodwar->drawCircleMap(target->getPosition().x(), target->getPosition().y(), 8, BWAPI::Colors::Red, true );
		Broodwar->drawCircleMap(target->getPosition().x(), target->getPosition().y(), this->microparam.targetNearby, BWAPI::Colors::Grey, false );
	}
	//red line
	if(unit->getHitPoints() > 0 && target && target->getHitPoints() > 0){
		Broodwar->drawLineMap(unit->getPosition().x(), unit->getPosition().y(), target->getPosition().x(), target->getPosition().y(), Colors::Red);
	}
}

void MicroAspect::dodge(std::set<BWAPI::Unit*> &enemies){
	Unit* enemy = this->getEnemyAttackingThisUnit(enemies);
	if(enemy){
		Position kitingpos= this->getKitingPosition(this->unit, enemy);
		this->unit->move(kitingpos);
	}
}

BWAPI::Unit* MicroAspect::getEnemyAttackingThisUnit(std::set<BWAPI::Unit*> &enemies){
	for each (Unit* enemy in enemies){
		if(enemy->getTarget() == this->unit){
			return enemy;
		}
	}
	return NULL;
}

Position MicroAspect::getKitingPosition(Unit* unit, Unit* target){
	double x1 = unit->getPosition().x();
	double y1 = unit->getPosition().y();
	double x2 = target->getPosition().x();
	double y2 = target->getPosition().y();

	double theta = std::atan2(y1-y2, x1-x2);
	//0.2 = 23 degree
	int x = x1+ this->microparam.kitingDist*cos(theta);
	int y = y1+ this->microparam.kitingDist*sin(theta);

	if(x<0) x=0;
	if(y<0) y=0;
	
	return Position(x, y);
}

Position MicroAspect::getKitingPositionByIM(Position* pos){
	Position kp = this->squad->im->getIMEnemy()->getHidingPos(pos, this->microparam.kitingDist);
	return kp;
}


bool MicroAspect::isBeingTarget(BWAPI::Unit* u, std::set<BWAPI::Unit*> &enemies){
	for each (Unit* enemy in enemies){
		Unit* tt = enemy->getTarget();
		Unit* tx = enemy->getOrderTarget();
		
		if(enemy->getTarget() == this->unit || enemy->getOrderTarget() == this->unit){
			Broodwar->drawLineMap(unit->getPosition().x(), unit->getPosition().y(), enemy->getPosition().x(), enemy->getPosition().y(), Colors::Blue);
			return true;
		}
	}
	return false;
}

void MicroAspect::updatePosition(){
	if(this->unit->getPosition().x() != this->position.second.x() || this->unit->getPosition().y() != this->position.second.y()){
		this->position = pair<int, BWAPI::Position>(Broodwar->getFrameCount(), this->unit->getPosition());
	}
}
bool MicroAspect::isUnitMoving(){
	if(Broodwar->getFrameCount() - this->position.first > 10){
		return false;
	}
	return true;
}

float MicroAspect::getUnitHPPercent(BWAPI::Unit* u){
	float hp = u->getHitPoints() + u->getShields();
	int max = u->getType().maxHitPoints() + u->getType().maxShields();
	return hp/max;
}

MicroAspect::~MicroAspect(){
}


