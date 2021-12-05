#pragma once
#include	"../gameobject/dice.h"
#include	"../gameobject/gameobject.h"
#include	"../../system/util/dixsmartptr.h"
#include	"../manager/stagedata_manager.h"

class DiceManager :Uncopyable
{
private:
	std::vector<Dice*> diceList;
	StageData mCurrentStageData;
	int diceMap[STAGESIZEMAX][STAGESIZEMAX];

	// Dice����
	void DiceCreate();

public:
	static DiceManager* GetInstance() {
		static DiceManager Instance;
		return &Instance;
	}
	DiceManager() {};
	~DiceManager() {};

	// ������
	void Init();
	void Update();
	void Uninit();

	// ���̃}�X��Dice�����݂��邩
	bool CanPush(Dice* _dice, DIRECTION _dire);
};

