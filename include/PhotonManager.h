#pragma once
#include <BWAPI.h>
#include <BaseManager.h>
#include <BuildManager.h>
#include <BuildOrderManager.h>
#include <BuildingPlacer.h>

class PhotonManager
{
public:
	void setBaseManager	(BaseManager*);
	void setBuildManager (BuildManager*);
	void setBuildOrderManager (BuildOrderManager*);
	void update();

private:
	BaseManager	*baseManager;
	BuildManager *buildManager;
	BuildOrderManager *buildOrderManager;
	int	lastFrameCheck;
};