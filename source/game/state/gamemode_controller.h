#pragma once
#include	"../../system/util/dixsmartptr.h"
#include	"gamemode_state_base.h"
#include	"../component/component_base.h"
#include	<map>

#define TOTALSTAGE (10)

class GameModeBase;

enum GameMode
{
	eSelect,
	ePuzzle,
	eEndless,
};

class GameModeController :public ComponentBase
{
private:
	std::map<int, Dix::sp<GameModeBase>> mStates;
	int mModeNum;
	int mSelectStage;

public:
	GameModeController();
	~GameModeController();

	void Awake()override {};
	void Init()override;
	void Update()override;
	void ImguiDraw()override;
	void Uninit()override {};

	void Init(int _modeNum);

	void ChangeMode(int _modeNum);

	int GetSelectStage() { return mSelectStage; }
	void GetSelectStage(int _num) { mSelectStage = mSelectStage; }
};

