#pragma once
#include	"../gameobject/dice.h"
#include	"../gameobject/gameobject.h"
#include	"../../system/util/dixsmartptr.h"
#include	"../manager/stagedata_manager.h"

class DiceManager :Uncopyable
{
private:
	std::vector<Dice*> mDiceList;
	StageData mCurrentStageData;
	int mDiceMap[STAGESIZEMAX][STAGESIZEMAX];

	// Diceê∂ê¨
	void DiceCreate();

public:
	static DiceManager* GetInstance() {
		static DiceManager Instance;
		return &Instance;
	}
	DiceManager() {};
	~DiceManager() {};

	// èâä˙âª
	void Init();
	void Update();
	void Uninit();

	// ÇªÇÃÉ}ÉXÇ…DiceÇ™ë∂ç›Ç∑ÇÈÇ©
	bool CanPush(Dice* _dice, Direction _dire);
};

