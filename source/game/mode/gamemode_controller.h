#pragma once
#include	"../../system/util/dixsmartptr.h"
#include	"gamemode_state_base.h"
#include	<map>

enum GameMode
{
	eSelect,
	ePuzzle,
	eEndless,
};

class GameModeController
{
private:
	std::map<int, Dix::sp<GameModeBase>> mStates;
	int mModeNum;
public:
	GameModeController() {};
	~GameModeController() {};

	void Init(int _modeNum);
	void Update();
	//void Draw();

	void ChangeMode(int _modeNum);
};

