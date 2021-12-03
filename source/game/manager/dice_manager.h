#pragma once
#include	"../gameobject/dice.h"
#include	"../gameobject/gameobject.h"
#include	"../../system/util/dixsmartptr.h"
#include	"../manager/stagedata_manager.h"

class DiceManager
{
private:
	std::vector<Dice*> diceList;
	StageData mCurrentStageData;
	int diceMap[STAGESIZEMAX][STAGESIZEMAX];

	// Dice生成
	void DiceCreate();
public:
	DiceManager() {};
	~DiceManager() {};

	// 初期化
	void Init();
	void Update();
	void Uninit();
};

