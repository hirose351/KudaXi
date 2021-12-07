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

	// Dice¶¬
	void DiceCreate();

public:
	static DiceManager* GetInstance() {
		static DiceManager Instance;
		return &Instance;
	}
	DiceManager() {};
	~DiceManager() {};

	// ‰Šú‰»
	void Init();
	void Update();
	void Uninit();

	// Dice‚ğˆÚ“®‚·‚é–‚ª‰Â”\‚©
	bool CanDiceMove(Dice* _dice, Direction _dire);
};

