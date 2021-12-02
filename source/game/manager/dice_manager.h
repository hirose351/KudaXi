#pragma once
#include	"../gameobject/dice.h"
#include	"../gameobject/gameobject.h"
#include	"../../system/util/dixsmartptr.h"
#include	"../manager/stagedata_manager.h"
#include	<list>

class DiceManager
{
private:
	std::list<Dix::sp<Dice>> diceList;
	int diceMap[STAGESIZEMAX][STAGESIZEMAX];
public:


};

